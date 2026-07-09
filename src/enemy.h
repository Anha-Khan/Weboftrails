#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"
#include "config.h"
#include <stdbool.h>

typedef enum EnemyDifficulty
{
    ENEMY_EASY,
    ENEMY_MEDIUM,
    ENEMY_HARD
} EnemyDifficulty;

// Simple state machine: walk up to the hero, telegraph a hit, swing,
// recover, repeat. Getting hit interrupts into a brief flinch.
typedef enum EnemyPhase
{
    ENEMY_APPROACH,
    ENEMY_TELEGRAPH,
    ENEMY_ATTACKING,
    ENEMY_RECOVER,
    ENEMY_HITSTUN,
    ENEMY_DEAD
} EnemyPhase;

typedef struct Enemy
{
    Vector2 position;
    int width;
    int height;
    int hp;
    int maxHp;
    int damage;
    float moveSpeed;
    float attackRange;
    float telegraphDuration;
    float cooldownDuration;
    float phaseTimer;
    bool facingRight;
    bool hasHitHeroThisAttack;
    EnemyDifficulty difficulty;
    EnemyPhase phase;
} Enemy;

Enemy EnemyCreate(Vector2 pos, EnemyDifficulty difficulty);
void EnemyUpdate(Enemy *enemy, float deltaTime, Vector2 heroCenter);
Rectangle EnemyGetRect(const Enemy *enemy);
Rectangle EnemyGetAttackRect(const Enemy *enemy); // only meaningful during ENEMY_ATTACKING
bool EnemyIsAttackActive(const Enemy *enemy);
void EnemyApplyDamage(Enemy *enemy, int amount);
bool EnemyIsAlive(const Enemy *enemy);
void EnemyDraw(const Enemy *enemy, float camX);

#endif
