#ifndef LEVEL1_H
#define LEVEL1_H

#include "raylib.h"
#include "config.h"
#include "hero.h"
#include "coin.h"
#include "obstacle.h"
#include <stdbool.h>

typedef struct Pit
{
    float x;
    float width;
} Pit;

typedef enum Level1State
{
    L1_COUNTDOWN,
    L1_PLAYING,
    L1_WIN,
    L1_LOSE
} Level1State;

typedef struct Level1
{
    Hero hero;
    float cameraX;
    float timeLeft;
    int coinsCollected;
    Level1State state;
    float countdownTimer;
    Coin coins[LEVEL1_COIN_COUNT];
    Obstacle obstacles[LEVEL1_OBS_COUNT];
    Pit pits[LEVEL1_PIT_COUNT];
    Texture2D coinTexture;
    Texture2D bgFar;
} Level1;

Level1 Level1Create(void);
void Level1Unload(Level1 *lvl);
void Level1Update(Level1 *lvl, float deltaTime);
void Level1Draw(const Level1 *lvl);

#endif