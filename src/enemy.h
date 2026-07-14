#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include <stdbool.h>

// ============================================================
// ENEMY.H / ENEMY.C
// The Level 2 villain. Mirrors hero.h/hero.c in structure:
// - EnemyCreate sets up starting state and loads its texture
// - EnemyUpdate ticks its attack timer and attacks every
//   VILLAIN_ATTACK_INTERVAL seconds
// - EnemyDraw renders it (falls back to a placeholder rectangle
//   if no art is loaded yet, same pattern as the hero)
// ============================================================

typedef struct Enemy {
    Vector2 position;
    int     width;
    int     height;

    int   hp;
    float attackTimer;   // counts down; villain attacks when it hits 0
    bool  justAttacked;  // true for one frame when an attack lands

    Texture2D idleTexture;
} Enemy;

// Creates the villain at a starting position with full HP.
Enemy EnemyCreate(Vector2 startPosition);

// Frees the villain's texture (call at program end).
void EnemyUnload(Enemy *enemy);

// Ticks the attack timer. When it reaches 0, the villain attacks
// (justAttacked becomes true for that frame) and the timer resets.
void EnemyUpdate(Enemy *enemy, float deltaTime);

// Applies damage to the villain (call this when the hero's attack lands).
void EnemyTakeDamage(Enemy *enemy, int damage);

// Draws the villain's current sprite (or placeholder) at its position.
void EnemyDraw(const Enemy *enemy);

#endif // ENEMY_H