#include "hero.h"
#include "config.h"

static Texture2D LoadSafe(const char *path) { return LoadTexture(path); }

Hero HeroCreate(Vector2 pos)
{
    Hero h = {0};
    h.position = pos;
    h.width = HERO_WIDTH;
    h.height = HERO_HEIGHT;
    h.facingRight = true;
    h.idleTexture = LoadSafe(HERO_IDLE_TEXTURE);
    h.runTexture = LoadSafe(HERO_RUN_TEXTURE);
    h.jumpTexture = LoadSafe(HERO_JUMP_TEXTURE);
    h.duckTexture = LoadSafe(HERO_DUCK_TEXTURE);
    return h;
}

void HeroUnload(Hero *h)
{
    if (h->idleTexture.id)
        UnloadTexture(h->idleTexture);
    if (h->runTexture.id)
        UnloadTexture(h->runTexture);
    if (h->jumpTexture.id)
        UnloadTexture(h->jumpTexture);
    if (h->duckTexture.id)
        UnloadTexture(h->duckTexture);
}

void HeroUpdate(Hero *h, float dt, float camX)
{
    // Duck
    bool wantDuck = (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) && h->isGrounded;
    if (wantDuck && !h->isDucking)
    {
        h->isDucking = true;
        h->position.y += (HERO_HEIGHT - HERO_DUCK_HEIGHT);
        h->height = HERO_DUCK_HEIGHT;
    }
    else if (!wantDuck && h->isDucking)
    {
        h->isDucking = false;
        h->position.y -= (HERO_HEIGHT - HERO_DUCK_HEIGHT);
        h->height = HERO_HEIGHT;
    }

    // Move
    h->velocity.x = 0;
    if (!h->isDucking)
    {
        if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
        {
            h->velocity.x = -MOVE_SPEED;
            h->facingRight = false;
        }
        if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
        {
            h->velocity.x = MOVE_SPEED;
            h->facingRight = true;
        }
    }

    // Jump
    if (!h->isDucking && h->isGrounded &&
        (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)))
    {
        h->velocity.y = JUMP_VELOCITY;
        h->isGrounded = false;
    }

    // Gravity
    h->velocity.y += GRAVITY * dt;

    // Apply
    h->position.x += h->velocity.x * dt;
    h->position.y += h->velocity.y * dt;

    // Ground
    if (h->position.y + h->height >= GROUND_Y)
    {
        h->position.y = GROUND_Y - h->height;
        h->velocity.y = 0;
        h->isGrounded = true;
    }

    // Boundaries
    if (h->position.x < camX)
        h->position.x = camX;
    if (h->position.x + h->width > camX + SCREEN_WIDTH)
        h->position.x = camX + SCREEN_WIDTH - h->width;
}

Rectangle HeroGetRect(const Hero *h)
{
    return (Rectangle){h->position.x, h->position.y,
                       (float)h->width, (float)h->height};
}

void HeroDraw(const Hero *h, float camX)
{
    float sx = h->position.x - camX;
    float sy = h->position.y;

    Texture2D t = h->idleTexture;
    if (h->isDucking)
        t = h->duckTexture;
    else if (!h->isGrounded)
        t = h->jumpTexture;
    else if (h->velocity.x)
        t = h->runTexture;

    if (t.id)
    {
        Rectangle src = {0, 0, (float)t.width, (float)t.height};
        if (!h->facingRight)
            src.width = -src.width;
        DrawTexturePro(t, src,
                       (Rectangle){sx, sy, (float)h->width, (float)h->height},
                       (Vector2){0, 0}, 0, WHITE);
    }
    else
    {
        Color c = h->isDucking ? YELLOW : (h->facingRight ? RED : BLUE);
        DrawRectangle((int)sx, (int)sy, h->width, h->height, c);
    }
}