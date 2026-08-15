#ifndef LEVEL1_H
#define LEVEL1_H

#include "raylib.h"
#include "config.h"
#include "hero.h"
#include "coin.h"
#include "obstacle.h"
#include "debris.h"
#include "difficulty.h"
#include "leaderboard.h"
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
    L1_LOSE,
    L1_ENTER_NAME,
    L1_LEADERBOARD
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
    FallingDebris debris[LEVEL1_DEBRIS_COUNT];
    Pit pits[LEVEL1_PIT_COUNT];
    Texture2D coinTexture;
    Texture2D bgFar;

    Difficulty difficulty;
    float runTime;             // seconds elapsed when the run was won
    bool qualifiesForBoard;
    char nameInput[LEADERBOARD_NAME_LEN];
    int nameInputLength;
    LeaderboardEntry board[LEADERBOARD_SIZE];
    int boardCount;

    Texture2D obsStaticTallTexture;
    Texture2D obsStaticLowTexture;
    Texture2D obsMovingTallTexture;
    Texture2D obsMovingLowTexture;
    Texture2D debrisTexture;
} Level1;

Level1 Level1Create(Difficulty difficulty);
void Level1Unload(Level1 *lvl);
void Level1Update(Level1 *lvl, float deltaTime);
void Level1Draw(const Level1 *lvl);
void Level1AdvanceFromWin(Level1 *lvl); // moves L1_WIN -> name entry or leaderboard

#endif