#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "raylib.h"
#include "config.h"
#include <stdbool.h>

typedef enum ObstacleType
{
    OBS_TALL,
    OBS_LOW
} ObstacleType;

typedef struct Obstacle
{
    Vector2 position;
    ObstacleType type;
    int width;
    int height;
    bool isMoving;
    float moveSpeed;
    float moveRange;
    float moveOrigin;
    float moveTimer;
    Texture2D texture; // shared, loaded once by Level1 - see ObstacleCreate
} Obstacle;

// `texture` is loaded once by the caller (Level1Create) and passed in by
// value - Obstacle does not own it and must not unload it. This matches
// how obstacle instances share art per-type instead of each loading its
// own copy of the same file.
Obstacle ObstacleCreate(Vector2 pos, ObstacleType type, bool isMoving, Texture2D texture);
void ObstacleUpdate(Obstacle *obs, float dt);
Rectangle ObstacleGetRect(const Obstacle *obs);
void ObstacleDraw(const Obstacle *obs, float camX);

#endif