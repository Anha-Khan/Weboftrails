#include "level1.h"
#include "config.h"
#include <stdio.h>

static void PlaceCoins(Level1 *lvl)
{
    float startX = 300.0f;
    float endX = LEVEL1_LENGTH - 200.0f;
    float spacing = (endX - startX) / LEVEL1_COIN_COUNT;

    for (int i = 0; i < LEVEL1_COIN_COUNT; i++)
    {
        float x = startX + i * spacing;
        float y = (i % 3 == 2) ? COIN_FLOAT_Y - 80.0f : COIN_FLOAT_Y;
        lvl->coins[i] = CoinCreate((Vector2){x, y}, lvl->coinTexture);
    }
}

static void PlaceObstacles(Level1 *lvl)
{
    float startX = 500.0f;
    float endX = LEVEL1_LENGTH - 300.0f;
    float spacing = (endX - startX) / LEVEL1_OBS_COUNT;

    for (int i = 0; i < LEVEL1_OBS_COUNT; i++)
    {
        float x = startX + i * spacing;
        ObstacleType type = (i % 2 == 0) ? OBS_TALL : OBS_LOW;
        lvl->obstacles[i] = ObstacleCreate((Vector2){x, 0}, type);
    }
}

Level1 Level1Create(void)
{
    Level1 lvl = {0};
    lvl.timeLeft = LEVEL1_TIME;
    lvl.cameraX = 0.0f;
    lvl.state = L1_PLAYING;
    lvl.coinTexture = LoadTexture(COIN_TEXTURE);
    lvl.hero = HeroCreate((Vector2){100.0f, GROUND_Y - HERO_HEIGHT});
    PlaceCoins(&lvl);
    PlaceObstacles(&lvl);
    return lvl;
}

void Level1Unload(Level1 *lvl)
{
    HeroUnload(&lvl->hero);
    if (lvl->coinTexture.id)
        UnloadTexture(lvl->coinTexture);
}

void Level1Update(Level1 *lvl, float dt)
{
    if (lvl->state != L1_PLAYING)
        return;

    // Timer
    lvl->timeLeft -= dt;
    if (lvl->timeLeft <= 0.0f)
    {
        lvl->timeLeft = 0.0f;
        lvl->state = L1_LOSE;
        return;
    }

    // Hero
    HeroUpdate(&lvl->hero, dt, lvl->cameraX);

    // Camera
    float targetCamX = lvl->hero.position.x - SCREEN_WIDTH * 0.3f;
    if (targetCamX < 0)
        targetCamX = 0;
    if (targetCamX > LEVEL1_LENGTH - SCREEN_WIDTH)
        targetCamX = LEVEL1_LENGTH - SCREEN_WIDTH;
    lvl->cameraX = targetCamX;

    Rectangle heroRect = HeroGetRect(&lvl->hero);

    // Coin collection
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

    // Obstacle collision
    for (int i = 0; i < LEVEL1_OBS_COUNT; i++)
    {
        Rectangle obsRect = ObstacleGetRect(&lvl->obstacles[i]);
        if (CheckCollisionRecs(heroRect, obsRect))
        {
            lvl->hero.position.x = obsRect.x - lvl->hero.width - 1.0f;
            lvl->hero.velocity.x = 0.0f;
        }
    }

    // Win check
    if (lvl->hero.position.x + lvl->hero.width >= LEVEL1_LENGTH)
        lvl->state = (lvl->coinsCollected >= LEVEL1_MIN_COINS) ? L1_WIN : L1_LOSE;
}

void Level1Draw(const Level1 *lvl)
{
    ClearBackground(SKYBLUE);

    // Background strips
    DrawRectangle(0, 200, SCREEN_WIDTH, 200, (Color){135, 180, 220, 255});
    DrawRectangle(0, 400, SCREEN_WIDTH, 200, (Color){100, 160, 80, 255});
    DrawRectangle(0, (int)GROUND_Y, SCREEN_WIDTH,
                  SCREEN_HEIGHT - (int)GROUND_Y, DARKGREEN);

    // Coins and obstacles
    for (int i = 0; i < LEVEL1_COIN_COUNT; i++)
        CoinDraw(&lvl->coins[i], lvl->cameraX);
    for (int i = 0; i < LEVEL1_OBS_COUNT; i++)
        ObstacleDraw(&lvl->obstacles[i], lvl->cameraX);

    // End marker
    float endX = LEVEL1_LENGTH - lvl->cameraX;
    if (endX >= 0 && endX <= SCREEN_WIDTH)
    {
        DrawRectangle((int)endX, (int)GROUND_Y - 120, 10, 120, GOLD);
        DrawText("END", (int)endX - 10, (int)GROUND_Y - 145, 20, GOLD);
    }

    // Hero
    HeroDraw(&lvl->hero, lvl->cameraX);

    // HUD
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

    // Win overlay
    if (lvl->state == L1_WIN)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 160});
        DrawText("LEVEL COMPLETE!", SCREEN_WIDTH / 2 - 180, SCREEN_HEIGHT / 2 - 60, 52, GREEN);
        char txt[64];
        sprintf(txt, "Coins collected: %d", lvl->coinsCollected);
        DrawText(txt, SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 10, 28, WHITE);
        DrawText("Press ENTER to continue", SCREEN_WIDTH / 2 - 160,
                 SCREEN_HEIGHT / 2 + 60, 26, YELLOW);
    }

    // Lose overlay
    if (lvl->state == L1_LOSE)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 160});
        DrawText("TIME'S UP!", SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT / 2 - 60, 52, RED);
        char txt[64];
        sprintf(txt, "Coins: %d  (need %d)", lvl->coinsCollected, LEVEL1_MIN_COINS);
        DrawText(txt, SCREEN_WIDTH / 2 - 130, SCREEN_HEIGHT / 2 + 10, 28, WHITE);
        DrawText("Press R to try again", SCREEN_WIDTH / 2 - 130,
                 SCREEN_HEIGHT / 2 + 60, 26, YELLOW);
    }
}