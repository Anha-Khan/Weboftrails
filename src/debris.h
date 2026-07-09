#ifndef DEBRIS_H
#define DEBRIS_H

#include "raylib.h"
#include "config.h"
#include <stdbool.h>

// Falling debris is a different kind of hazard than Obstacle:
// obstacles are timed jump/duck checks that stay put, debris is a
// telegraphed hazard that drops straight down onto a fixed spot and
// has to be dodged sideways (move away in X before it lands).
typedef enum DebrisState
{
    DEBRIS_WARNING, // sitting up top, ground marker pulsing as a warning
    DEBRIS_FALLING, // actively dropping - touching it here is a hit
    DEBRIS_LANDED   // harmless rubble on the ground, about to reset
} DebrisState;

typedef struct FallingDebris
{
    float x; // fixed landing spot, doesn't move horizontally
    float y;
    int width;
    int height;
    DebrisState state;
    float timer;
} FallingDebris;

FallingDebris DebrisCreate(float x, float initialTimerOffset);
void DebrisUpdate(FallingDebris *debris, float dt);
Rectangle DebrisGetRect(const FallingDebris *debris);
bool DebrisIsDeadly(const FallingDebris *debris);
void DebrisDraw(const FallingDebris *debris, float camX);

#endif
