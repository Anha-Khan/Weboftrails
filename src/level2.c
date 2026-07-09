#include "level2.h"
#include "config.h"
#include <stdio.h>
#include <math.h>

Level2 Level2Create(void)
{
    Level2 lvl = {0};
    lvl.state = L2_COUNTDOWN;
    lvl.countdownTimer = COUNTDOWN_DURATION;
    lvl.bgFar = LoadTexture(BG_FAR_TEXTURE);
    lvl.hero = HeroCreate((Vector2){100.0f, GROUND_Y - HERO_HEIGHT});
    HeroCombatReset(&lvl.hero);

    float triggerXs[LEVEL2_ARENA_COUNT] = {900.0f, 2150.0f, 3400.0f};
    EnemyDifficulty diffs[LEVEL2_ARENA_COUNT] = {ENEMY_EASY, ENEMY_MEDIUM, ENEMY_HARD};

    for (int i = 0; i < LEVEL2_ARENA_COUNT; i++)
    {
        lvl.arenas[i].triggerX = triggerXs[i];
        lvl.arenas[i].active = false;
        lvl.arenas[i].cleared = false;
        Vector2 enemyPos = {triggerXs[i] + ARENA_ENEMY_OFFSET, GROUND_Y - ENEMY_HEIGHT};
        lvl.arenas[i].enemy = EnemyCreate(enemyPos, diffs[i]);
    }
    return lvl;
}

void Level2Unload(Level2 *lvl)
{
    HeroUnload(&lvl->hero);
    if (lvl->bgFar.id)
        UnloadTexture(lvl->bgFar);
}

void Level2Update(Level2 *lvl, float dt)
{
    if (lvl->state == L2_COUNTDOWN)
    {
        lvl->countdownTimer -= dt;
        if (lvl->countdownTimer <= 0.0f)
        {
            lvl->countdownTimer = 0.0f;
            lvl->state = L2_PLAYING;
        }
        return;
    }

    if (lvl->state != L2_PLAYING)
        return;

    HeroUpdate(&lvl->hero, dt, lvl->cameraX);
    HeroCombatUpdate(&lvl->hero, dt);

    // No pits in level 2 - always solid ground.
    float heroFeet = lvl->hero.position.y + lvl->hero.height;
    if (heroFeet >= GROUND_Y)
    {
        lvl->hero.position.y = GROUND_Y - lvl->hero.height;
        lvl->hero.velocity.y = 0.0f;
        lvl->hero.isGrounded = true;
    }

    // Fights happen one at a time, in order. Find the first uncleared
    // arena - it's either the one currently being fought, or the next
    // one the hero is walking toward.
    int activeIndex = -1;
    for (int i = 0; i < LEVEL2_ARENA_COUNT; i++)
    {
        if (lvl->arenas[i].cleared)
            continue;
        if (!lvl->arenas[i].active &&
            lvl->hero.position.x + lvl->hero.width >= lvl->arenas[i].triggerX)
        {
            lvl->arenas[i].active = true;
        }
        if (lvl->arenas[i].active)
            activeIndex = i;
        break;
    }

    if (activeIndex >= 0)
    {
        Arena *a = &lvl->arenas[activeIndex];

        // Lock the camera on the arena and keep the hero from running
        // past it (or backing all the way out) until it's cleared.
        float lockedCamX = a->triggerX - ARENA_CAMERA_LEAD;
        if (lockedCamX < 0.0f)
            lockedCamX = 0.0f;
        if (lockedCamX > LEVEL2_LENGTH - SCREEN_WIDTH)
            lockedCamX = LEVEL2_LENGTH - SCREEN_WIDTH;
        lvl->cameraX = lockedCamX;

        float leftBound = a->triggerX - 40.0f;
        float rightBound = a->triggerX + ARENA_WIDTH - lvl->hero.width;
        if (lvl->hero.position.x < leftBound)
            lvl->hero.position.x = leftBound;
        if (lvl->hero.position.x > rightBound)
            lvl->hero.position.x = rightBound;

        Vector2 heroCenter = {lvl->hero.position.x + lvl->hero.width * 0.5f,
                              lvl->hero.position.y + lvl->hero.height * 0.5f};
        EnemyUpdate(&a->enemy, dt, heroCenter);

        Rectangle heroRect = HeroGetRect(&lvl->hero);

        if (lvl->hero.isAttacking && !lvl->hero.hasHitThisSwing && EnemyIsAlive(&a->enemy))
        {
            if (CheckCollisionRecs(HeroGetAttackRect(&lvl->hero), EnemyGetRect(&a->enemy)))
            {
                EnemyApplyDamage(&a->enemy, HERO_ATTACK_DAMAGE);
                lvl->hero.hasHitThisSwing = true;
            }
        }

        if (EnemyIsAttackActive(&a->enemy) && !a->enemy.hasHitHeroThisAttack)
        {
            if (CheckCollisionRecs(EnemyGetAttackRect(&a->enemy), heroRect))
            {
                HeroTakeDamage(&lvl->hero, a->enemy.damage);
                a->enemy.hasHitHeroThisAttack = true;
            }
        }

        if (!EnemyIsAlive(&a->enemy))
        {
            a->cleared = true;
            a->active = false;
        }

        if (lvl->hero.hp <= 0)
        {
            lvl->state = L2_LOSE;
            return;
        }
    }
    else
    {
        float targetCamX = lvl->hero.position.x - SCREEN_WIDTH * 0.3f;
        if (targetCamX < 0.0f)
            targetCamX = 0.0f;
        if (targetCamX > LEVEL2_LENGTH - SCREEN_WIDTH)
            targetCamX = LEVEL2_LENGTH - SCREEN_WIDTH;
        lvl->cameraX = targetCamX;
    }

    if (lvl->hero.position.x + lvl->hero.width >= LEVEL2_LENGTH)
        lvl->state = L2_WIN;
}

static void DrawHpBar(float x, float y, float width, float height, int hp, int maxHp)
{
    DrawRectangle((int)x, (int)y, (int)width, (int)height, (Color){40, 40, 40, 220});
    float pct = (maxHp > 0) ? (float)hp / (float)maxHp : 0.0f;
    if (pct < 0)
        pct = 0;
    Color c = (pct > 0.5f) ? GREEN : (pct > 0.2f ? ORANGE : RED);
    DrawRectangle((int)x, (int)y, (int)(width * pct), (int)height, c);
}

void Level2Draw(const Level2 *lvl)
{
    ClearBackground((Color){35, 20, 45, 255});

    if (lvl->bgFar.id)
    {
        float scale = (float)SCREEN_HEIGHT / lvl->bgFar.height;
        float bgW = lvl->bgFar.width * scale;
        float bgX = fmodf(-(lvl->cameraX * 0.3f), bgW);
        if (bgX > 0)
            bgX -= bgW;
        Color tint = (Color){170, 160, 190, 255};
        DrawTextureEx(lvl->bgFar, (Vector2){bgX, 0}, 0, scale, tint);
        DrawTextureEx(lvl->bgFar, (Vector2){bgX + bgW, 0}, 0, scale, tint);
        DrawTextureEx(lvl->bgFar, (Vector2){bgX + bgW * 2, 0}, 0, scale, tint);
    }

    DrawRectangle(0, (int)GROUND_Y, SCREEN_WIDTH, SCREEN_HEIGHT - (int)GROUND_Y,
                  (Color){50, 38, 60, 255});

    for (int i = 0; i < LEVEL2_ARENA_COUNT; i++)
        EnemyDraw(&lvl->arenas[i].enemy, lvl->cameraX);

    HeroDraw(&lvl->hero, lvl->cameraX);

    if (lvl->hero.isAttacking)
    {
        Rectangle atk = HeroGetAttackRect(&lvl->hero);
        DrawRectangle((int)(atk.x - lvl->cameraX), (int)atk.y, (int)atk.width, (int)atk.height,
                      (Color){255, 255, 255, 160});
    }

    float hsx = lvl->hero.position.x - lvl->cameraX;
    float barW = 60.0f;
    float barX = hsx + lvl->hero.width * 0.5f - barW * 0.5f;
    float barY = lvl->hero.position.y - 18;
    DrawHpBar(barX, barY, barW, 7, lvl->hero.hp, lvl->hero.maxHp);
    if (lvl->hero.isBlocking)
        DrawText("BLOCK", (int)barX - 4, (int)(barY - 16), 14, SKYBLUE);
    else if (lvl->hero.isDodging)
        DrawText("DODGE", (int)barX - 4, (int)(barY - 16), 14, YELLOW);

    DrawText("J = attack   K = block   L = dodge   A/D = move   SPACE = jump",
             20, SCREEN_HEIGHT - 30, 18, LIGHTGRAY);

    char hpText[32];
    sprintf(hpText, "HP: %d / %d", lvl->hero.hp, lvl->hero.maxHp);
    DrawText(hpText, 20, 20, 28, WHITE);

    int clearedCount = 0;
    for (int i = 0; i < LEVEL2_ARENA_COUNT; i++)
        if (lvl->arenas[i].cleared)
            clearedCount++;
    char progText[40];
    sprintf(progText, "Enemies defeated: %d / %d", clearedCount, LEVEL2_ARENA_COUNT);
    DrawText(progText, 20, 54, 22, LIGHTGRAY);

    DrawFPS(SCREEN_WIDTH - 90, 10);

    if (lvl->state == L2_COUNTDOWN)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 120});
        int count = (int)lvl->countdownTimer + 1;
        char countText[4];
        sprintf(countText, "%d", count);
        DrawText(countText, SCREEN_WIDTH / 2 - MeasureText(countText, 120) / 2,
                 SCREEN_HEIGHT / 2 - 80, 120, WHITE);
        DrawText("FIGHT!", SCREEN_WIDTH / 2 - MeasureText("FIGHT!", 36) / 2,
                 SCREEN_HEIGHT / 2 + 60, 36, YELLOW);
    }

    if (lvl->state == L2_WIN)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 160});
        DrawText("LEVEL 2 COMPLETE!",
                 SCREEN_WIDTH / 2 - MeasureText("LEVEL 2 COMPLETE!", 52) / 2,
                 SCREEN_HEIGHT / 2 - 60, 52, GREEN);
        DrawText("Level 3 is coming soon.",
                 SCREEN_WIDTH / 2 - MeasureText("Level 3 is coming soon.", 26) / 2,
                 SCREEN_HEIGHT / 2 + 10, 26, WHITE);
        DrawText("Press ENTER to fight again",
                 SCREEN_WIDTH / 2 - MeasureText("Press ENTER to fight again", 26) / 2,
                 SCREEN_HEIGHT / 2 + 60, 26, YELLOW);
    }

    if (lvl->state == L2_LOSE)
    {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 160});
        DrawText("DEFEATED!", SCREEN_WIDTH / 2 - MeasureText("DEFEATED!", 52) / 2,
                 SCREEN_HEIGHT / 2 - 60, 52, RED);
        DrawText("Press R to try again",
                 SCREEN_WIDTH / 2 - MeasureText("Press R to try again", 26) / 2,
                 SCREEN_HEIGHT / 2 + 60, 26, YELLOW);
    }
}
