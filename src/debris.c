#include "debris.h"
#include "config.h"
#include <math.h>

FallingDebris DebrisCreate(float x, float initialTimerOffset, Texture2D texture)
{
    FallingDebris d = {0};
    d.x = x;
    d.y = DEBRIS_START_Y;
    d.width = DEBRIS_WIDTH;
    d.height = DEBRIS_HEIGHT;
    d.state = DEBRIS_WARNING;
    d.texture = texture;
    // Stagger start so multiple debris along the level don't all drop
    // in lockstep - each one is offset within its own warning window.
    d.timer = initialTimerOffset;
    return d;
}

void DebrisUpdate(FallingDebris *d, float dt)
{
    switch (d->state)
    {
    case DEBRIS_WARNING:
        d->timer += dt;
        if (d->timer >= DEBRIS_WARNING_DURATION)
        {
            d->state = DEBRIS_FALLING;
            d->y = DEBRIS_START_Y;
            d->timer = 0.0f;
        }
        break;

    case DEBRIS_FALLING:
        d->y += DEBRIS_FALL_SPEED * dt;
        if (d->y + d->height >= GROUND_Y)
        {
            d->y = GROUND_Y - d->height;
            d->state = DEBRIS_LANDED;
            d->timer = 0.0f;
        }
        break;

    case DEBRIS_LANDED:
        d->timer += dt;
        if (d->timer >= DEBRIS_LANDED_DURATION)
        {
            d->state = DEBRIS_WARNING;
            d->timer = 0.0f;
            d->y = DEBRIS_START_Y;
        }
        break;
    }
}

Rectangle DebrisGetRect(const FallingDebris *d)
{
    return (Rectangle){d->x, d->y, (float)d->width, (float)d->height};
}

bool DebrisIsDeadly(const FallingDebris *d)
{
    return d->state == DEBRIS_FALLING;
}

void DebrisDraw(const FallingDebris *d, float camX)
{
    float sx = d->x - camX;
    if (sx + d->width < 0 || sx > SCREEN_WIDTH)
        return;

    if (d->state == DEBRIS_WARNING)
    {
        // Pulsing ground marker + "!" only - no rectangle outline.
        float ratio = d->timer / DEBRIS_WARNING_DURATION;
        float pulse = 0.5f + 0.5f * fabsf(sinf(ratio * 18.0f));
        unsigned char alpha = (unsigned char)(90 + pulse * 140);
        Color warnColor = (Color){220, 40, 40, alpha};

        DrawRectangle((int)sx, (int)(GROUND_Y - 6), d->width, 6, warnColor);
        DrawText("!", (int)(sx + d->width / 2 - 4), (int)(GROUND_Y - d->height - 22),
                 22, warnColor);
    }
    else if (d->state == DEBRIS_FALLING)
    {
        if (d->texture.id)
            DrawTexturePro(d->texture,
                           (Rectangle){0, 0, (float)d->texture.width, (float)d->texture.height},
                           (Rectangle){sx, d->y, (float)d->width, (float)d->height},
                           (Vector2){0, 0}, 0, WHITE);
        else
        {
            DrawRectangle((int)sx, (int)d->y, d->width, d->height, (Color){120, 90, 70, 255});
            DrawRectangleLinesEx(
                (Rectangle){sx, d->y, (float)d->width, (float)d->height}, 2, (Color){70, 50, 40, 255});
        }
    }
    else // DEBRIS_LANDED
    {
        float shrink = d->timer / DEBRIS_LANDED_DURATION;
        int h = (int)(d->height * (1.0f - 0.5f * shrink));
        if (d->texture.id)
            DrawTexturePro(d->texture,
                           (Rectangle){0, 0, (float)d->texture.width, (float)d->texture.height},
                           (Rectangle){sx, GROUND_Y - h, (float)d->width, (float)h},
                           (Vector2){0, 0}, 0, (Color){255, 255, 255, 200});
        else
            DrawRectangle((int)sx, (int)(GROUND_Y - h), d->width, h, (Color){100, 75, 60, 200});
    }
}