#ifndef DEBRIS_H
#define DEBRIS_H

#include "raylib.h"
#include "config.h"
#include <stdbool.h>

typedef enum DebrisState
{
    DEBRIS_WARNING,
    DEBRIS_FALLING,
    DEBRIS_LANDED
} DebrisState;

typedef struct FallingDebris
{
    float x;
    float y;
    int width;
    int height;
    DebrisState state;
    float timer;
    Texture2D texture; // shared, loaded once by Level1 - see DebrisCreate
} FallingDebris;

// `texture` is loaded once by the caller (Level1Create) and passed in by
// value - FallingDebris does not own it and must not unload it.
FallingDebris DebrisCreate(float x, float initialTimerOffset, Texture2D texture);
void DebrisUpdate(FallingDebris *d, float dt);
Rectangle DebrisGetRect(const FallingDebris *d);
bool DebrisIsDeadly(const FallingDebris *d);
void DebrisDraw(const FallingDebris *d, float camX);

#endif