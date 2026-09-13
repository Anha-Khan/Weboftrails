#include "obstacle.h"
#include "config.h"
#include <math.h>

Obstacle ObstacleCreate(Vector2 pos, bool isMoving, Texture2D texture)
{
    Obstacle obs = {0};
    obs.position = pos;
    obs.isMoving = isMoving;
    obs.texture = texture;

    obs.width = OBS_LOW_WIDTH;
    obs.height = OBS_LOW_HEIGHT;
    obs.position.y = GROUND_Y - OBS_LOW_HEIGHT;
    obs.moveOrigin = GROUND_Y - OBS_LOW_HEIGHT;
    return obs;
}

void ObstacleUpdate(Obstacle *obs, float dt)
{
    if (!obs->isMoving)
        return;
    obs->moveTimer += dt;
    obs->position.y = obs->moveOrigin -
                      fabsf(sinf(obs->moveTimer * OBS_MOVE_SPEED * 0.015f)) * OBS_MOVE_RANGE;
    if (obs->position.y < 50)
        obs->position.y = 50;
}

Rectangle ObstacleGetRect(const Obstacle *obs)
{
    return (Rectangle){obs->position.x, obs->position.y,
                       (float)obs->width, (float)obs->height};
}

void ObstacleDraw(const Obstacle *obs, float camX)
{
    float sx = obs->position.x - camX;
    float sy = obs->position.y;

    if (sx + obs->width < 0 || sx > SCREEN_WIDTH)
        return;

    if (obs->texture.id)
    {
        DrawTexturePro(obs->texture,
                       (Rectangle){0, 0, (float)obs->texture.width, (float)obs->texture.height},
                       (Rectangle){sx, sy, (float)obs->width, (float)obs->height},
                       (Vector2){0, 0}, 0, WHITE);
        return;
    }

    // Fallback if the texture failed to load.
    DrawRectangle((int)sx, (int)sy, obs->width, obs->height, PURPLE);
    DrawText("BLOCK", (int)sx + 2, (int)sy + obs->height / 2 - 8, 12, WHITE);
}
