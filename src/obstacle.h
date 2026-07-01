#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "raylib.h"
#include "config.h"

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
} Obstacle;

Obstacle ObstacleCreate(Vector2 pos, ObstacleType type);
Rectangle ObstacleGetRect(const Obstacle *obs);
void ObstacleDraw(const Obstacle *obs, float camX);

#endif