#ifndef HERO_H
#define HERO_H

#include "raylib.h"
#include <stdbool.h>

typedef struct Hero
{
    Vector2 position;
    Vector2 velocity;
    int width;
    int height;
    bool isGrounded;
    bool facingRight;
    bool isDucking;
    Texture2D idleTexture;
    Texture2D runTextures[3];
    int runFrameCount;
    int runFrameIndex;
    float runFrameTimer;
    Texture2D jumpTexture;
    Texture2D duckTexture;

    // Combat (used in Level 2 - harmless/unused in Level 1)
    int hp;
    int maxHp;
    bool isAttacking;
    bool hasHitThisSwing;
    float attackTimer;
    float attackCooldownTimer;
    bool isBlocking;
    bool isDodging;
    float dodgeTimer;
    float dodgeCooldownTimer;
    float invulnTimer;
} Hero;

Hero HeroCreate(Vector2 startPosition);
void HeroUnload(Hero *hero);
void HeroUpdate(Hero *hero, float deltaTime, float cameraX);
void HeroDraw(const Hero *hero, float cameraX);
Rectangle HeroGetRect(const Hero *hero);

// Combat (Level 2)
void HeroCombatReset(Hero *hero);
void HeroCombatUpdate(Hero *hero, float deltaTime);
Rectangle HeroGetAttackRect(const Hero *hero);
void HeroTakeDamage(Hero *hero, int amount);
bool HeroIsInvulnerable(const Hero *hero);

#endif
