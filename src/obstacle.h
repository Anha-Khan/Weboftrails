#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "raylib.h"
#include <stdbool.h>

typedef struct Obstacle
{
    Vector2 position;
    int width;
    int height;
    bool isMoving;
    float moveOrigin;
    float moveTimer;
    Texture2D texture;
} Obstacle;

Obstacle ObstacleCreate(Vector2 pos, bool isMoving, Texture2D texture);
void ObstacleUpdate(Obstacle *obs, float dt);
Rectangle ObstacleGetRect(const Obstacle *obs);
void ObstacleDraw(const Obstacle *obs, float camX);

#endif
