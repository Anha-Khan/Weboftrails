#ifndef HERO_H
#define HERO_H

#include "raylib.h"
#include <stdbool.h>

// ============================================================
// HERO.H / HERO.C
// Everything about our playable character lives here.
// ============================================================

typedef struct Hero {
    Vector2 position;
    Vector2 velocity;
    int     width;
    int     height;
    bool    isGrounded;
    bool    facingRight;

    // ---- Combat (Level 2) ----
    int   hp;
    bool  isAttacking;
    float attackCooldownTimer;
    bool  isDodging;
    float dodgeTimer;
    float dodgeCooldownTimer;

    Texture2D idleTexture;
    Texture2D runTexture;
    Texture2D jumpTexture;
} Hero;

Hero HeroCreate(Vector2 startPosition);
void HeroUnload(Hero *hero);
void HeroUpdate(Hero *hero, float deltaTime);
void HeroDraw(const Hero *hero);
void HeroUpdateCombat(Hero *hero, float deltaTime);
void HeroTakeDamage(Hero *hero, int damage);

#endif // HERO_H