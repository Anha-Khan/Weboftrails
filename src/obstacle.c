#include "obstacle.h"
#include "config.h"

Obstacle ObstacleCreate(Vector2 pos, ObstacleType type)
{
    Obstacle obs = {0};
    obs.position = pos;
    obs.type = type;

    if (type == OBS_TALL)
    {
        obs.width = OBS_TALL_WIDTH;
        obs.height = OBS_TALL_HEIGHT;
        obs.position.y = GROUND_Y - OBS_TALL_HEIGHT;
    }
    else
    {
        obs.width = OBS_LOW_WIDTH;
        obs.height = OBS_LOW_HEIGHT;
        obs.position.y = GROUND_Y - OBS_LOW_HEIGHT;
    }
    return obs;
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

    if (obs->type == OBS_TALL)
    {
        DrawRectangle((int)sx, (int)sy, obs->width, obs->height, DARKBROWN);
        DrawText("JUMP", (int)sx + 2, (int)sy + obs->height / 2 - 8, 12, WHITE);
    }
    else
    {
        DrawRectangle((int)sx, (int)sy, obs->width, obs->height, PURPLE);
        DrawText("DUCK", (int)sx + 4, (int)sy + obs->height / 2 - 8, 12, WHITE);
    }
}