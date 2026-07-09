#ifndef LEVEL2_H
#define LEVEL2_H

#include "raylib.h"
#include "config.h"
#include "hero.h"
#include "enemy.h"
#include <stdbool.h>

#define LEVEL2_ARENA_COUNT 3

typedef enum Level2State
{
    L2_COUNTDOWN,
    L2_PLAYING,
    L2_WIN,
    L2_LOSE
} Level2State;

// A fight zone: the hero can't run past `triggerX + ARENA_WIDTH` until
// the enemy here is defeated. Arenas are fought in array order
// (easy -> medium -> hard).
typedef struct Arena
{
    float triggerX;
    Enemy enemy;
    bool active;
    bool cleared;
} Arena;

typedef struct Level2
{
    Hero hero;
    float cameraX;
    Level2State state;
    float countdownTimer;
    Arena arenas[LEVEL2_ARENA_COUNT];
    Texture2D bgFar;
} Level2;

Level2 Level2Create(void);
void Level2Unload(Level2 *lvl);
void Level2Update(Level2 *lvl, float deltaTime);
void Level2Draw(const Level2 *lvl);

#endif
