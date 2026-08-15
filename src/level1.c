#include "level1.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

// True if a hazard of [x, x+width] would land within `margin` pixels of
// another hazard occupying [otherX, otherX+otherWidth]. Used to keep
// pits/obstacles/debris from stacking on top of each other so there's
// always room to react to one hazard before the next one shows up.
static bool RangeTooClose(float x, float width, float otherX, float otherWidth, float margin)
{
    return (x - margin) < (otherX + otherWidth) && (x + width + margin) > otherX;
}

static void PlacePits(Level1 *lvl)
{
    float startX = 600.0f;
    float endX = LEVEL1_LENGTH - 400.0f;
    float zone = (endX - startX) / LEVEL1_PIT_COUNT;
    for (int i = 0; i < LEVEL1_PIT_COUNT; i++)
    {
        float zoneStart = startX + i * zone;
        float width = LEVEL1_PIT_MIN_W +
                      (rand() % (int)(LEVEL1_PIT_MAX_W - LEVEL1_PIT_MIN_W));
        float x = zoneStart + (rand() % (int)(zone * 0.6f));
        lvl->pits[i].x = x;
        lvl->pits[i].width = width;
    }
}

static void PlaceCoins(Level1 *lvl)
{
    float startX = 300.0f;
    float endX = LEVEL1_LENGTH - 200.0f;
    float spacing = (endX - startX) / LEVEL1_COIN_COUNT;
    for (int i = 0; i < LEVEL1_COIN_COUNT; i++)
    {
        float x = startX + i * spacing;
        float y = (i % 3 == 2) ? COIN_FLOAT_Y - 80.0f : COIN_FLOAT_Y;
        bool inPit = false;
        for (int p = 0; p < LEVEL1_PIT_COUNT; p++)
        {
            if (x >= lvl->pits[p].x &&
                x <= lvl->pits[p].x + lvl->pits[p].width)
            {
                inPit = true;
                break;
            }
        }
        if (inPit)
            x -= 60.0f;
        lvl->coins[i] = CoinCreate((Vector2){x, y}, lvl->coinTexture);
    }
}

static void PlaceObstacles(Level1 *lvl)
{
    float startX = 500.0f;
    float endX = LEVEL1_LENGTH - 300.0f;
    float spacing = (endX - startX) / LEVEL1_OBS_COUNT;
    // Gap must be at least the hero's own width, or there's no room to
    // stand between two hazards - 150 is the normal design buffer, but
    // this guarantees it never drops below HERO_WIDTH even if that
    // constant changes later.
    float margin = fmaxf((float)HERO_WIDTH, 150.0f);

    float prevObsEnd = -1.0f; // end of the last placed obstacle; -1 = none yet

    for (int i = 0; i < LEVEL1_OBS_COUNT; i++)
    {
        float zoneStart = startX + i * spacing;
        float x = zoneStart;
        ObstacleType type = (i % 2 == 0) ? OBS_TALL : OBS_LOW;
        float width = (type == OBS_TALL) ? OBS_TALL_WIDTH : OBS_LOW_WIDTH;

        int attempts = 0;
        while (attempts < 20)
        {
            bool conflict = false;
            for (int p = 0; p < LEVEL1_PIT_COUNT; p++)
            {
                if (RangeTooClose(x, width, lvl->pits[p].x, lvl->pits[p].width, margin))
                {
                    conflict = true;
                    break;
                }
            }
            if (!conflict && prevObsEnd >= 0.0f && x - margin < prevObsEnd)
                conflict = true;
            if (!conflict)
                break;
            x += 50.0f;
            attempts++;
        }

        float zoneEnd = zoneStart + spacing;
        if (x + width > zoneEnd)
            x = zoneEnd - width - 10.0f;
        if (prevObsEnd >= 0.0f && x - margin < prevObsEnd)
            x = prevObsEnd + margin;

        bool isMoving = (i % 3 == 0);
        lvl->obstacles[i] = ObstacleCreate((Vector2){x, 0}, type, isMoving);
        prevObsEnd = x + width;
    }
}

static void PlaceDebris(Level1 *lvl)
{
    float startX = 700.0f;
    float endX = LEVEL1_LENGTH - 400.0f;
    float spacing = (endX - startX) / LEVEL1_DEBRIS_COUNT;
    float margin = fmaxf((float)HERO_WIDTH, 200.0f);

    float prevDebrisEnd = -1.0f;

    for (int i = 0; i < LEVEL1_DEBRIS_COUNT; i++)
    {
        float zoneStart = startX + i * spacing;
        float x = zoneStart + (rand() % (int)(spacing * 0.3f));

        int attempts = 0;
        while (attempts < 20)
        {
            bool conflict = false;
            for (int p = 0; p < LEVEL1_PIT_COUNT && !conflict; p++)
                if (RangeTooClose(x, DEBRIS_WIDTH, lvl->pits[p].x, lvl->pits[p].width, margin))
                    conflict = true;
            for (int o = 0; o < LEVEL1_OBS_COUNT && !conflict; o++)
                if (RangeTooClose(x, DEBRIS_WIDTH, lvl->obstacles[o].position.x,
                                  (float)lvl->obstacles[o].width, margin))
                    conflict = true;
            if (!conflict && prevDebrisEnd >= 0.0f && x - margin < prevDebrisEnd)
                conflict = true;
            if (!conflict)
                break;
            x += 50.0f;
            attempts++;
        }

        float zoneEnd = zoneStart + spacing;
        if (x + DEBRIS_WIDTH > zoneEnd)
            x = zoneEnd - DEBRIS_WIDTH - 10.0f;
        if (prevDebrisEnd >= 0.0f && x - margin < prevDebrisEnd)
            x = prevDebrisEnd + margin;

        float offset = (float)(rand() % 1000) / 1000.0f * DEBRIS_WARNING_DURATION;
        lvl->debris[i] = DebrisCreate(x, offset);
        prevDebrisEnd = x + DEBRIS_WIDTH;
    }
}

Level1 Level1Create(Difficulty difficulty)
{
    srand((unsigned int)time(NULL));
    Level1 lvl = {0};
    lvl.timeLeft = LEVEL1_TIME;
    lvl.cameraX = 0.0f;
    lvl.state = L1_COUNTDOWN;
    lvl.countdownTimer = COUNTDOWN_DURATION;
    lvl.coinTexture = LoadTexture(COIN_TEXTURE);
    lvl.bgFar = LoadTexture(BG_FAR_TEXTURE);
    lvl.difficulty = difficulty;
    lvl.hero = HeroCreate((Vector2){100.0f, GROUND_Y - HERO_HEIGHT},
                           DifficultySpeedMultiplier(difficulty));
    lvl.nameInputLength = 0;
    memset(lvl.nameInput, 0, sizeof(lvl.nameInput));
    LeaderboardLoad(lvl.board, &lvl.boardCount);
    PlacePits(&lvl);
    PlaceCoins(&lvl);
    PlaceObstacles(&lvl);
    PlaceDebris(&lvl);
    return lvl;
}

void Level1Unload(Level1 *lvl)
{
    HeroUnload(&lvl->hero);
    if (lvl->coinTexture.id)
        UnloadTexture(lvl->coinTexture);
    if (lvl->bgFar.id)
        UnloadTexture(lvl->bgFar);
}

void Level1AdvanceFromWin(Level1 *lvl)
{
    if (lvl->state != L1_WIN)
        return;
    if (lvl->qualifiesForBoard)
    {
        lvl->nameInputLength = 0;
        memset(lvl->nameInput, 0, sizeof(lvl->nameInput));
        lvl->state = L1_ENTER_NAME;
    }
    else
    {
        lvl->state = L1_LEADERBOARD;
    }
}

void Level1Update(Level1 *lvl, float dt)
{
    if (lvl->state == L1_COUNTDOWN)
    {
        lvl->countdownTimer -= dt;
        if (lvl->countdownTimer <= 0.0f)
        {
            lvl->countdownTimer = 0.0f;
            lvl->state = L1_PLAYING;
        }
        return;
    }

    if (lvl->state == L1_ENTER_NAME)
    {
        int ch = GetCharPressed();
        while (ch > 0)
        {
            if (ch >= 32 && ch <= 125 && ch != ',' &&
                lvl->nameInputLength < LEADERBOARD_NAME_LEN - 1)
            {
                lvl->nameInput[lvl->nameInputLength] = (char)ch;
                lvl->nameInputLength++;
                lvl->nameInput[lvl->nameInputLength] = '\0';
            }
            ch = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && lvl->nameInputLength > 0)
        {
            lvl->nameInputLength--;
            lvl->nameInput[lvl->nameInputLength] = '\0';
        }
        if (IsKeyPressed(KEY_ENTER) && lvl->nameInputLength > 0)
        {
            LeaderboardEntry entry = {0};
            strncpy(entry.name, lvl->nameInput, LEADERBOARD_NAME_LEN - 1);
            entry.coins = lvl->coinsCollected;
            entry.time = lvl->runTime;
            LeaderboardInsert(lvl->board, &lvl->boardCount, entry);
            LeaderboardSave(lvl->board, lvl->boardCount);
            lvl->state = L1_LEADERBOARD;
        }
        return;
    }

    if (lvl->state != L1_PLAYING)
        return;

    lvl->timeLeft -= dt;
    if (lvl->timeLeft <= 0.0f)
    {
        lvl->timeLeft = 0.0f;
        lvl->state = L1_LOSE;
        return;
    }

    HeroUpdate(&lvl->hero, dt, lvl->cameraX);

    float heroLeft = lvl->hero.position.x;
    float heroRight = lvl->hero.position.x + lvl->hero.width;
    bool overPit = false;
    for (int i = 0; i < LEVEL1_PIT_COUNT; i++)
    {
        float pitLeft = lvl->pits[i].x;
        float pitRight = lvl->pits[i].x + lvl->pits[i].width;
        if (heroRight > pitLeft && heroLeft < pitRight)
        {
            overPit = true;
            break;
        }
    }

    if (overPit)
    {
        lvl->hero.isGrounded = false;
        if (lvl->hero.position.y > SCREEN_HEIGHT + 100)
        {
            lvl->state = L1_LOSE;
            return;
        }
    }
    else
    {
        float heroFeet = lvl->hero.position.y + lvl->hero.height;
        if (heroFeet >= GROUND_Y)
        {
            lvl->hero.position.y = GROUND_Y - lvl->hero.height;
            lvl->hero.velocity.y = 0;
            lvl->hero.isGrounded = true;
        }
    }

    float targetCamX = lvl->hero.position.x - SCREEN_WIDTH * 0.3f;
    if (targetCamX < 0)
        targetCamX = 0;
    if (targetCamX > LEVEL1_LENGTH - SCREEN_WIDTH)
        targetCamX = LEVEL1_LENGTH - SCREEN_WIDTH;
    lvl->cameraX = targetCamX;

    Rectangle heroRect = HeroGetRect(&lvl->hero);

    for (int i = 0; i < LEVEL1_OBS_COUNT; i++)
        ObstacleUpdate(&lvl->obstacles[i], dt);

    for (int i = 0; i < LEVEL1_OBS_COUNT; i++)
    {
        Rectangle obsRect = ObstacleGetRect(&lvl->obstacles[i]);
        if (!CheckCollisionRecs(heroRect, obsRect))
            continue;
        lvl->hero.position.x = obsRect.x - lvl->hero.width - 1.0f;
        lvl->hero.velocity.x = 0.0f;
    }

    for (int i = 0; i < LEVEL1_COIN_COUNT; i++)
    {
        if (lvl->coins[i].collected)
            continue;
        if (CheckCollisionRecs(heroRect, CoinGetRect(&lvl->coins[i])))
        {
            lvl->coins[i].collected = true;
            lvl->coinsCollected++;
        }
    }

    for (int i = 0; i < LEVEL1_DEBRIS_COUNT; i++)
        DebrisUpdate(&lvl->debris[i], dt);

    for (int i = 0; i < LEVEL1_DEBRIS_COUNT; i++)
    {
        if (!DebrisIsDeadly(&lvl->debris[i]))
            continue;
        if (CheckCollisionRecs(heroRect, DebrisGetRect(&lvl->debris[i])))
        {
            lvl->state = L1_LOSE;
            return;
        }
    }

    if (lvl->hero.position.x + lvl->hero.width >= LEVEL1_LENGTH)
    {
        if (lvl->coinsCollected >= LEVEL1_MIN_COINS)
        {
            lvl->runTime = LEVEL1_TIME - lvl->timeLeft;
            lvl->qualifiesForBoard = LeaderboardQualifies(
                lvl->board, lvl->boardCount, lvl->coinsCollected, lvl->runTime);
            lvl->state = L1_WIN;
        }
        else
        {
            lvl->state = L1_LOSE;
        }
    }
}

void Level1Draw(const Level1 *lvl)
{
    ClearBackground(SKYBLUE);

    if (lvl->bgFar.id)
    {
        float scale = (float)SCREEN_HEIGHT / lvl->bgFar.height;
        float bgW = lvl->bgFar.width * scale;
        float bgX = fmodf(-(lvl->cameraX * 0.3f), bgW);
        if (bgX > 0)
            bgX -= bgW;
        DrawTextureEx(lvl->bgFar, (Vector2){bgX, 0}, 0, scale, WHITE);
        DrawTextureEx(lvl->bgFar, (Vector2){bgX + bgW, 0}, 0, scale, WHITE);
        DrawTextureEx(lvl->bgFar, (Vector2){bgX + bgW * 2, 0}, 0, scale, WHITE);
    }
    else
    {
        DrawRectangle(0, 200, SCREEN_WIDTH, 200, (Color){135, 180, 220, 255});
        DrawRectangle(0, 400, SCREEN_WIDTH, 200, (Color){100, 160, 80, 255});
    }

    float groundSegStart = 0;
    for (int i = 0; i < LEVEL1_PIT_COUNT; i++)
    {
        float pitScreenX = lvl->pits[i].x - lvl->cameraX;
        float segWidth = pitScreenX - groundSegStart;
        if (segWidth > 0)
            DrawRectangle((int)groundSegStart, (int)GROUND_Y,
                          (int)segWidth,
                          SCREEN_HEIGHT - (int)GROUND_Y, DARKGREEN);
        groundSegStart = pitScreenX + lvl->pits[i].width;
    }
    float remaining = SCREEN_WIDTH - groundSegStart;
    if (remaining > 0)
        DrawRectangle((int)groundSegStart, (int)GROUND_Y,
                      (int)remaining,
                      SCREEN_HEIGHT - (int)GROUND_Y, DARKGREEN);

    for (int i = 0; i < LEVEL1_PIT_COUNT; i++)
    {
        float sx = lvl->pits[i].x - lvl->cameraX;
        if (sx + lvl->pits[i].width < 0 || sx > SCREEN_WIDTH)
            continue;
        DrawRectangle((int)sx, (int)GROUND_Y,
                      (int)lvl->pits[i].width,
                      SCREEN_HEIGHT - (int)GROUND_Y,
                      (Color){20, 20, 20, 255});
    }

    for (int i = 0; i < LEVEL1_COIN_COUNT; i++)
        CoinDraw(&lvl->coins[i], lvl->cameraX);
    for (int i = 0; i < LEVEL1_OBS_COUNT; i++)
        ObstacleDraw(&lvl->obstacles[i], lvl->cameraX);
    for (int i = 0; i < LEVEL1_DEBRIS_COUNT; i++)
        DebrisDraw(&lvl->debris[i], lvl->cameraX);

    float endX = LEVEL1_LENGTH - lvl->cameraX;
    if (endX >= 0 && endX <= SCREEN_WIDTH)
    {
        DrawRectangle((int)endX, (int)GROUND_Y - 120, 10, 120, GOLD);
        DrawText("END", (int)endX - 10, (int)GROUND_Y - 145, 20, GOLD);
    }

    HeroDraw(&lvl->hero, lvl->cameraX);

    char timerText[32];
    sprintf(timerText, "TIME: %d", (int)lvl->timeLeft);
    Color timerColor = (lvl->timeLeft <= 10.0f) ? RED : WHITE;
    DrawText(timerText, SCREEN_WIDTH / 2 - 60, 20, 36, timerColor);

    char coinText[32];
    sprintf(coinText, "COINS: %d / %d", lvl->coinsCollected, LEVEL1_MIN_COINS);
    Color coinColor = (lvl->coinsCollected >= LEVEL1_MIN_COINS) ? GREEN : WHITE;
    DrawText(coinText, 20, 20, 28, coinColor);

    char diffText[32];
    sprintf(diffText, "%s", DifficultyName(lvl->difficulty));
    DrawText(diffText, SCREEN_WIDTH - 120, 20, 22, LIGHTGRAY);

    DrawText("A/D = move   SPACE = jump   S = duck",
             20, SCREEN_HEIGHT - 30, 18, LIGHTGRAY);
    DrawFPS(SCREEN_WIDTH - 90, 60);

    if (lvl->state == L1_COUNTDOWN)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 120});
        int count = (int)lvl->countdownTimer + 1;
        char countText[4];
        sprintf(countText, "%d", count);
        DrawText(countText,
                 SCREEN_WIDTH / 2 - MeasureText(countText, 120) / 2,
                 SCREEN_HEIGHT / 2 - 80, 120, WHITE);
        DrawText("GET READY!",
                 SCREEN_WIDTH / 2 - MeasureText("GET READY!", 36) / 2,
                 SCREEN_HEIGHT / 2 + 60, 36, YELLOW);
    }

    if (lvl->state == L1_WIN)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 160});
        DrawText("LEVEL COMPLETE!",
                 SCREEN_WIDTH / 2 - MeasureText("LEVEL COMPLETE!", 52) / 2,
                 SCREEN_HEIGHT / 2 - 60, 52, GREEN);
        char txt[64];
        sprintf(txt, "Coins: %d   Time: %.1fs", lvl->coinsCollected, lvl->runTime);
        DrawText(txt, SCREEN_WIDTH / 2 - MeasureText(txt, 28) / 2,
                 SCREEN_HEIGHT / 2 + 10, 28, WHITE);
        const char *prompt = lvl->qualifiesForBoard
                                  ? "NEW HIGH SCORE! Press ENTER to enter your name"
                                  : "Press ENTER to continue";
        Color promptColor = lvl->qualifiesForBoard ? GOLD : YELLOW;
        DrawText(prompt, SCREEN_WIDTH / 2 - MeasureText(prompt, 24) / 2,
                 SCREEN_HEIGHT / 2 + 60, 24, promptColor);
    }

    if (lvl->state == L1_ENTER_NAME)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 170});
        DrawText("NEW HIGH SCORE!",
                 SCREEN_WIDTH / 2 - MeasureText("NEW HIGH SCORE!", 44) / 2,
                 SCREEN_HEIGHT / 2 - 110, 44, GOLD);
        char stats[64];
        sprintf(stats, "Coins: %d   Time: %.1fs", lvl->coinsCollected, lvl->runTime);
        DrawText(stats, SCREEN_WIDTH / 2 - MeasureText(stats, 24) / 2,
                 SCREEN_HEIGHT / 2 - 50, 24, WHITE);
        DrawText("Enter your name:",
                 SCREEN_WIDTH / 2 - MeasureText("Enter your name:", 22) / 2,
                 SCREEN_HEIGHT / 2 - 5, 22, LIGHTGRAY);
        char display[LEADERBOARD_NAME_LEN + 2];
        sprintf(display, "%s_", lvl->nameInput);
        DrawText(display, SCREEN_WIDTH / 2 - MeasureText(display, 32) / 2,
                 SCREEN_HEIGHT / 2 + 30, 32, YELLOW);
        DrawText("Press ENTER to confirm",
                 SCREEN_WIDTH / 2 - MeasureText("Press ENTER to confirm", 18) / 2,
                 SCREEN_HEIGHT / 2 + 90, 18, LIGHTGRAY);
    }

    if (lvl->state == L1_LEADERBOARD)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 180});
        DrawText("TOP 5", SCREEN_WIDTH / 2 - MeasureText("TOP 5", 44) / 2,
                 100, 44, GOLD);
        int y = 190;
        for (int i = 0; i < lvl->boardCount; i++)
        {
            char row[80];
            sprintf(row, "%d. %-12s  %2d coins  %.1fs",
                    i + 1, lvl->board[i].name, lvl->board[i].coins, lvl->board[i].time);
            DrawText(row, SCREEN_WIDTH / 2 - MeasureText(row, 24) / 2, y, 24, WHITE);
            y += 40;
        }
        if (lvl->boardCount == 0)
        {
            DrawText("No scores yet - be the first!",
                     SCREEN_WIDTH / 2 - MeasureText("No scores yet - be the first!", 22) / 2,
                     y, 22, LIGHTGRAY);
            y += 40;
        }
        DrawText("Press ENTER to play again",
                 SCREEN_WIDTH / 2 - MeasureText("Press ENTER to play again", 24) / 2,
                 y + 40, 24, YELLOW);
    }

    if (lvl->state == L1_LOSE)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 160});
        const char *reason = (lvl->hero.position.y > SCREEN_HEIGHT)
                                 ? "YOU FELL!"
                                 : "GAME OVER!";
        DrawText(reason,
                 SCREEN_WIDTH / 2 - MeasureText(reason, 52) / 2,
                 SCREEN_HEIGHT / 2 - 60, 52, RED);
        char txt[64];
        sprintf(txt, "Coins: %d  (need %d)",
                lvl->coinsCollected, LEVEL1_MIN_COINS);
        DrawText(txt, SCREEN_WIDTH / 2 - MeasureText(txt, 28) / 2,
                 SCREEN_HEIGHT / 2 + 10, 28, WHITE);
        DrawText("Press R to try again",
                 SCREEN_WIDTH / 2 - MeasureText("Press R to try again", 26) / 2,
                 SCREEN_HEIGHT / 2 + 60, 26, YELLOW);
    }
}