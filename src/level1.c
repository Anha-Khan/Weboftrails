#include "level1.h"
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
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
    float margin = 150.0f; // room to land/recover before the next hazard

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

            // Also keep distance from the obstacle placed right before this
            // one. Without this check, a DUCK and a JUMP obstacle (which
            // need opposite moves) could land right on top of each other
            // with no gap to react between them - impossible to pass.
            if (!conflict && i > 0)
            {
                Obstacle *prev = &lvl->obstacles[i - 1];
                if (RangeTooClose(x, width, prev->position.x, (float)prev->width, margin))
                    conflict = true;
            }

            if (!conflict)
                break;
            x += 50.0f;
            attempts++;
        }

        float zoneEnd = zoneStart + spacing;
        if (x + width > zoneEnd)
            x = zoneEnd - width - 10.0f;

        // Last-resort safety net: if the zone clamp above pushed x back
        // into the previous obstacle's space, force a clean gap instead
        // of allowing an unpassable pair through.
        if (i > 0)
        {
            Obstacle *prev = &lvl->obstacles[i - 1];
            float minX = prev->position.x + (float)prev->width + margin;
            if (x < minX)
                x = minX;
        }

        bool isMoving = (i % 3 == 0);
        lvl->obstacles[i] = ObstacleCreate((Vector2){x, 0}, type, isMoving);
    }
}

static void PlaceDebris(Level1 *lvl)
{
    float startX = 700.0f;
    float endX = LEVEL1_LENGTH - 400.0f;
    float spacing = (endX - startX) / LEVEL1_DEBRIS_COUNT;
    float margin = 200.0f; // room to dodge sideways before/after any other hazard

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
            if (!conflict)
                break;
            x += 50.0f;
            attempts++;
        }

        float zoneEnd = zoneStart + spacing;
        if (x + DEBRIS_WIDTH > zoneEnd)
            x = zoneEnd - DEBRIS_WIDTH - 10.0f;

        // Stagger each one's warning timer so they don't all drop together.
        float offset = (float)(rand() % 1000) / 1000.0f * DEBRIS_WARNING_DURATION;
        lvl->debris[i] = DebrisCreate(x, offset);
    }
}

Level1 Level1Create(void)
{
    srand((unsigned int)time(NULL));
    Level1 lvl = {0};
    lvl.timeLeft = LEVEL1_TIME;
    lvl.cameraX = 0.0f;
    lvl.state = L1_COUNTDOWN;
    lvl.countdownTimer = COUNTDOWN_DURATION;
    lvl.coinTexture = LoadTexture(COIN_TEXTURE);
    lvl.bgFar = LoadTexture(BG_FAR_TEXTURE);
    lvl.hero = HeroCreate((Vector2){100.0f, GROUND_Y - HERO_HEIGHT});
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

    // Pit check: any horizontal overlap between the hero and a pit means
    // there's no ground under them there (previously this required the
    // hero to be *entirely* inside the pit, so they'd visibly float over
    // most of the gap before suddenly dropping once fully swallowed by it).
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

    // Gravity + vertical movement already happened once inside HeroUpdate.
    // We only decide here whether the hero is standing on solid ground or
    // falling through a pit - we must NOT integrate gravity/position again,
    // that was applying gravity twice per frame while over a pit.
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
        // Obstacles (moving or static) just block your path - they don't
        // instantly end the level. The only instant-death hazards are
        // falling into a pit and getting hit by falling debris. Moving
        // obstacles used to insta-kill on "crushedFromAbove", but since
        // they bob up into the air on their own, that could trigger mid-
        // jump (e.g. right after clearing a pit) in a way that looked and
        // felt like landing on open ground - confusing and unfair.
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
        lvl->state = (lvl->coinsCollected >= LEVEL1_MIN_COINS)
                         ? L1_WIN
                         : L1_LOSE;
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

    // Ground with pits
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

    DrawText("A/D = move   SPACE = jump   S = duck",
             20, SCREEN_HEIGHT - 30, 18, LIGHTGRAY);
    DrawFPS(SCREEN_WIDTH - 90, 10);

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
        sprintf(txt, "Coins collected: %d", lvl->coinsCollected);
        DrawText(txt, SCREEN_WIDTH / 2 - MeasureText(txt, 28) / 2,
                 SCREEN_HEIGHT / 2 + 10, 28, WHITE);
        DrawText("Press ENTER to continue",
                 SCREEN_WIDTH / 2 - MeasureText("Press ENTER to continue", 26) / 2,
                 SCREEN_HEIGHT / 2 + 60, 26, YELLOW);
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
