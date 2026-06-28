#include "hero.h"
#include "config.h"

// Small helper: tries to load a texture; if the file doesn't exist yet,
// raylib will print a warning but won't crash. We check afterwards and
// fall back to drawing a colored rectangle in HeroDraw() instead.
static Texture2D LoadTextureSafe(const char *path) {
    Texture2D tex = LoadTexture(path);
    return tex; // tex.id will be 0 if loading failed - we check this later
}

Hero HeroCreate(Vector2 startPosition) {
    Hero hero = { 0 };

    hero.position    = startPosition;
    hero.velocity    = (Vector2){ 0, 0 };
    hero.width       = HERO_WIDTH;
    hero.height      = HERO_HEIGHT;
    hero.isGrounded  = false;
    hero.facingRight = true;

    hero.idleTexture = LoadTextureSafe(HERO_IDLE_TEXTURE);
    hero.runTexture  = LoadTextureSafe(HERO_RUN_TEXTURE);
    hero.jumpTexture = LoadTextureSafe(HERO_JUMP_TEXTURE);

    return hero;
}

void HeroUnload(Hero *hero) {
    // Only unload if it actually loaded (id != 0), otherwise raylib
    // would print an unnecessary warning about unloading nothing.
    if (hero->idleTexture.id != 0) UnloadTexture(hero->idleTexture);
    if (hero->runTexture.id  != 0) UnloadTexture(hero->runTexture);
    if (hero->jumpTexture.id != 0) UnloadTexture(hero->jumpTexture);
}

void HeroUpdate(Hero *hero, float deltaTime) {
    // ---- Horizontal movement ----
    hero->velocity.x = 0.0f;

    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
        hero->velocity.x = -MOVE_SPEED;
        hero->facingRight = false;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
        hero->velocity.x = MOVE_SPEED;
        hero->facingRight = true;
    }

    // ---- Jumping ----
    // Only allowed if currently standing on the ground.
    if (hero->isGrounded && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W))) {
        hero->velocity.y = JUMP_VELOCITY;
        hero->isGrounded = false;
    }

    // ---- Gravity ----
    // Always pulling down, unless we're standing on the ground.
    hero->velocity.y += GRAVITY * deltaTime;

    // ---- Apply velocity to position ----
    hero->position.x += hero->velocity.x * deltaTime;
    hero->position.y += hero->velocity.y * deltaTime;

    // ---- Ground collision (simple flat floor for now) ----
    float feetY = hero->position.y + hero->height;
    if (feetY >= GROUND_Y) {
        hero->position.y = GROUND_Y - hero->height;
        hero->velocity.y = 0.0f;
        hero->isGrounded = true;
    }

    // ---- Keep hero inside the screen horizontally (temporary) ----
    if (hero->position.x < 0) hero->position.x = 0;
    if (hero->position.x + hero->width > SCREEN_WIDTH) {
        hero->position.x = SCREEN_WIDTH - hero->width;
    }
}

void HeroDraw(const Hero *hero) {
    // Pick which texture should be showing based on current state.
    Texture2D current = hero->idleTexture;
    if (!hero->isGrounded) {
        current = hero->jumpTexture;
    } else if (hero->velocity.x != 0.0f) {
        current = hero->runTexture;
    }

    if (current.id != 0) {
        // We have real art - draw it, flipped if facing left.
        Rectangle source = { 0, 0, (float)current.width, (float)current.height };
        if (!hero->facingRight) source.width = -source.width; // flip horizontally

        Rectangle dest = {
            hero->position.x, hero->position.y,
            (float)hero->width, (float)hero->height
        };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(current, source, dest, origin, 0.0f, WHITE);
    } else {
        // No art yet - draw a placeholder rectangle so the game is
        // still fully playable. Red = facing right, blue = facing left,
        // just so you can visually confirm direction-flipping works.
        Color placeholderColor = hero->facingRight ? RED : BLUE;
        DrawRectangle(
            (int)hero->position.x, (int)hero->position.y,
            hero->width, hero->height,
            placeholderColor
        );
    }
}
