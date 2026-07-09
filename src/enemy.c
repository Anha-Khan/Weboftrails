#include "enemy.h"
#include "config.h"
#include <math.h>

Enemy EnemyCreate(Vector2 pos, EnemyDifficulty difficulty)
{
    Enemy e = {0};
    e.position = pos;
    e.width = ENEMY_WIDTH;
    e.height = ENEMY_HEIGHT;
    e.difficulty = difficulty;
    e.phase = ENEMY_APPROACH;
    e.facingRight = false; // enemies stand to the right of the hero, face left

    switch (difficulty)
    {
    case ENEMY_EASY:
        e.maxHp = ENEMY_EASY_HP;
        e.damage = ENEMY_EASY_DAMAGE;
        e.moveSpeed = ENEMY_EASY_SPEED;
        e.attackRange = ENEMY_EASY_RANGE;
        e.telegraphDuration = ENEMY_EASY_TELEGRAPH;
        e.cooldownDuration = ENEMY_EASY_COOLDOWN;
        break;
    case ENEMY_MEDIUM:
        e.maxHp = ENEMY_MEDIUM_HP;
        e.damage = ENEMY_MEDIUM_DAMAGE;
        e.moveSpeed = ENEMY_MEDIUM_SPEED;
        e.attackRange = ENEMY_MEDIUM_RANGE;
        e.telegraphDuration = ENEMY_MEDIUM_TELEGRAPH;
        e.cooldownDuration = ENEMY_MEDIUM_COOLDOWN;
        break;
    case ENEMY_HARD:
        e.maxHp = ENEMY_HARD_HP;
        e.damage = ENEMY_HARD_DAMAGE;
        e.moveSpeed = ENEMY_HARD_SPEED;
        e.attackRange = ENEMY_HARD_RANGE;
        e.telegraphDuration = ENEMY_HARD_TELEGRAPH;
        e.cooldownDuration = ENEMY_HARD_COOLDOWN;
        break;
    }
    e.hp = e.maxHp;
    return e;
}

void EnemyUpdate(Enemy *e, float dt, Vector2 heroCenter)
{
    if (e->phase == ENEMY_DEAD)
        return;

    Vector2 myCenter = {e->position.x + e->width * 0.5f, e->position.y + e->height * 0.5f};
    float dx = heroCenter.x - myCenter.x;
    e->facingRight = dx > 0;

    switch (e->phase)
    {
    case ENEMY_APPROACH:
    {
        float dist = fabsf(dx);
        if (dist > e->attackRange)
        {
            float dir = (dx > 0) ? 1.0f : -1.0f;
            e->position.x += dir * e->moveSpeed * dt;
        }
        else
        {
            e->phase = ENEMY_TELEGRAPH;
            e->phaseTimer = 0.0f;
        }
        break;
    }
    case ENEMY_TELEGRAPH:
        e->phaseTimer += dt;
        if (e->phaseTimer >= e->telegraphDuration)
        {
            e->phase = ENEMY_ATTACKING;
            e->phaseTimer = 0.0f;
            e->hasHitHeroThisAttack = false;
        }
        break;
    case ENEMY_ATTACKING:
        e->phaseTimer += dt;
        if (e->phaseTimer >= ENEMY_ATTACK_ACTIVE_DURATION)
        {
            e->phase = ENEMY_RECOVER;
            e->phaseTimer = 0.0f;
        }
        break;
    case ENEMY_RECOVER:
        e->phaseTimer += dt;
        if (e->phaseTimer >= e->cooldownDuration)
        {
            e->phase = ENEMY_APPROACH;
            e->phaseTimer = 0.0f;
        }
        break;
    case ENEMY_HITSTUN:
        e->phaseTimer += dt;
        if (e->phaseTimer >= ENEMY_HITSTUN_DURATION)
        {
            e->phase = ENEMY_APPROACH;
            e->phaseTimer = 0.0f;
        }
        break;
    case ENEMY_DEAD:
        break;
    }
}

Rectangle EnemyGetRect(const Enemy *e)
{
    return (Rectangle){e->position.x, e->position.y, (float)e->width, (float)e->height};
}

bool EnemyIsAttackActive(const Enemy *e)
{
    return e->phase == ENEMY_ATTACKING;
}

Rectangle EnemyGetAttackRect(const Enemy *e)
{
    float range = 40.0f;
    float x = e->facingRight ? (e->position.x + e->width) : (e->position.x - range);
    return (Rectangle){x, e->position.y, range, (float)e->height};
}

void EnemyApplyDamage(Enemy *e, int amount)
{
    if (e->phase == ENEMY_DEAD)
        return;
    e->hp -= amount;
    if (e->hp <= 0)
    {
        e->hp = 0;
        e->phase = ENEMY_DEAD;
        return;
    }
    e->phase = ENEMY_HITSTUN;
    e->phaseTimer = 0.0f;
}

bool EnemyIsAlive(const Enemy *e)
{
    return e->phase != ENEMY_DEAD;
}

static Color DifficultyColor(EnemyDifficulty d)
{
    switch (d)
    {
    case ENEMY_EASY:
        return (Color){80, 170, 90, 255};
    case ENEMY_MEDIUM:
        return (Color){210, 140, 40, 255};
    case ENEMY_HARD:
    default:
        return (Color){170, 40, 40, 255};
    }
}

void EnemyDraw(const Enemy *e, float camX)
{
    if (e->phase == ENEMY_DEAD)
        return;

    float sx = e->position.x - camX;
    if (sx + e->width < 0 || sx > SCREEN_WIDTH)
        return;

    Color body = DifficultyColor(e->difficulty);
    if (e->phase == ENEMY_TELEGRAPH)
    {
        // Flash brighter as the swing gets closer - the dodge/block cue.
        float ratio = e->phaseTimer / e->telegraphDuration;
        float pulse = 0.5f + 0.5f * fabsf(sinf(ratio * 22.0f));
        body = (Color){255, (unsigned char)(60 + pulse * 100), (unsigned char)(60 + pulse * 100), 255};
    }
    else if (e->phase == ENEMY_ATTACKING)
    {
        body = (Color){255, 230, 60, 255};
    }
    else if (e->phase == ENEMY_HITSTUN)
    {
        body = WHITE;
    }

    DrawRectangle((int)sx, (int)e->position.y, e->width, e->height, body);

    if (e->phase == ENEMY_ATTACKING)
    {
        Rectangle atk = EnemyGetAttackRect(e);
        DrawRectangle((int)(atk.x - camX), (int)atk.y, (int)atk.width, (int)atk.height,
                      (Color){255, 255, 255, 140});
    }

    // HP bar above the head
    float barW = (float)e->width;
    float barX = sx;
    float barY = e->position.y - 14;
    DrawRectangle((int)barX, (int)barY, (int)barW, 6, (Color){40, 40, 40, 220});
    float pct = (e->maxHp > 0) ? (float)e->hp / (float)e->maxHp : 0.0f;
    Color hpColor = (pct > 0.5f) ? GREEN : (pct > 0.2f ? ORANGE : RED);
    DrawRectangle((int)barX, (int)barY, (int)(barW * pct), 6, hpColor);

    const char *label = (e->difficulty == ENEMY_EASY) ? "EASY" : (e->difficulty == ENEMY_MEDIUM ? "MEDIUM" : "HARD");
    DrawText(label, (int)sx, (int)(barY - 16), 14, WHITE);
}
