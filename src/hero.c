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
    h.runTextures[0] = LoadSafe(HERO_RUN_TEXTURE_1);
    h.runTextures[1] = LoadSafe(HERO_RUN_TEXTURE_2);
    h.runTextures[2] = LoadSafe(HERO_RUN_TEXTURE_3);
    h.runFrameCount = 3;
    h.jumpTexture = LoadSafe(HERO_JUMP_TEXTURE);
    h.duckTexture = LoadSafe(HERO_DUCK_TEXTURE);
    return h;
}

void HeroUnload(Hero *h)
{
    if (h->idleTexture.id)
        UnloadTexture(h->idleTexture);
    for (int i = 0; i < h->runFrameCount; i++)
        if (h->runTextures[i].id)
            UnloadTexture(h->runTextures[i]);
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

    // Run animation frame cycling (only while actually running on the ground)
    if (h->isGrounded && !h->isDucking && h->velocity.x != 0 && h->runFrameCount > 0)
    {
        h->runFrameTimer += dt;
        if (h->runFrameTimer >= RUN_FRAME_DURATION)
        {
            h->runFrameTimer = 0.0f;
            h->runFrameIndex = (h->runFrameIndex + 1) % h->runFrameCount;
        }
    }
    else
    {
        h->runFrameTimer = 0.0f;
        h->runFrameIndex = 0;
    }

    // Gravity
    h->velocity.y += GRAVITY * dt;

    // Apply
    h->position.x += h->velocity.x * dt;
    h->position.y += h->velocity.y * dt;

    // NOTE: ground/pit collision is resolved by Level1Update, not here.
    // Hero only knows about gravity and movement; the level knows about
    // terrain (ground vs. pits), so that's the single source of truth
    // for isGrounded/snapping. Doing it here too caused gravity to be
    // applied twice per frame while the hero was over a pit.

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
    else if (h->velocity.x && h->runFrameCount > 0)
        t = h->runTextures[h->runFrameIndex];

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