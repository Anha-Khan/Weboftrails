#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_TITLE "Web of Trails"
#define TARGET_FPS 60

#define HERO_IDLE_TEXTURE "assets/hero/hero_idle.png"
#define HERO_RUN_TEXTURE_1 "assets/hero/hero_run1.png"
#define HERO_RUN_TEXTURE_2 "assets/hero/hero_run2.png"
#define HERO_RUN_TEXTURE_3 "assets/hero/hero_run3.png"
#define HERO_JUMP_TEXTURE "assets/hero/hero_jump.png"
#define HERO_DUCK_TEXTURE "assets/hero/hero_duck.png"
#define COIN_TEXTURE "assets/levels/coin.png"
#define BG_FAR_TEXTURE "assets/backgrounds/bg_far.png"

#define RUN_FRAME_DURATION 0.09f

#define GRAVITY 1800.0f
#define MOVE_SPEED 420.0f
#define JUMP_VELOCITY -700.0f
#define GROUND_Y 600.0f

#define HERO_WIDTH 50
#define HERO_HEIGHT 80
#define HERO_DUCK_HEIGHT 40

<<<<<<< HEAD
// ---- Hero size (used until we load real sprite dimensions) ----
#define HERO_WIDTH         50
#define HERO_HEIGHT        80
// ---- Combat tuning (Level 2) ----
#define HERO_MAX_HP           100
#define HERO_ATTACK_DAMAGE    15
#define HERO_ATTACK_COOLDOWN  0.5f    // seconds between hero attacks
#define HERO_DODGE_DURATION   0.3f    // seconds the dodge lasts
#define HERO_DODGE_COOLDOWN   1.0f    // seconds before dodge can be used again

#define VILLAIN_MAX_HP        80
#define VILLAIN_ATTACK_DAMAGE 10
#define VILLAIN_ATTACK_INTERVAL 2.0f  // villain attacks every 2 seconds

// ---- Villain asset path (swap this file to change the villain's look) ----
#define VILLAIN_TEXTURE  "assets/enemies/villain_idle.png"
=======
#define LEVEL1_TIME 60.0f
#define LEVEL1_MIN_COINS 10
#define LEVEL1_LENGTH 8000.0f
#define LEVEL1_COIN_COUNT 20
#define LEVEL1_OBS_COUNT 15
#define LEVEL1_PIT_COUNT 6
#define LEVEL1_PIT_MIN_W 80.0f
#define LEVEL1_PIT_MAX_W 160.0f
>>>>>>> main

#define COIN_WIDTH 24
#define COIN_HEIGHT 24
#define COIN_FLOAT_Y (GROUND_Y - 80.0f)

#define OBS_TALL_WIDTH 40
#define OBS_TALL_HEIGHT 90
#define OBS_LOW_WIDTH 80
#define OBS_LOW_HEIGHT 35
#define OBS_MOVE_SPEED 180.0f
#define OBS_MOVE_RANGE 200.0f

#define COUNTDOWN_DURATION 3.0f

// Falling debris: hazards that drop from above and must be dodged
// sideways (unlike OBS_TALL/OBS_LOW, which are timed jump/duck checks).
#define LEVEL1_DEBRIS_COUNT 8
#define DEBRIS_WIDTH 46
#define DEBRIS_HEIGHT 46
#define DEBRIS_START_Y -150.0f
#define DEBRIS_FALL_SPEED 950.0f
#define DEBRIS_WARNING_DURATION 1.0f
#define DEBRIS_LANDED_DURATION 0.9f

// ---- Level 2: combat ----
// Controls: J = attack, K = block (hold), L = dodge (tap, brief i-frames)
#define KEY_ATTACK KEY_J
#define KEY_BLOCK KEY_K
#define KEY_DODGE KEY_L

#define LEVEL2_LENGTH 4200.0f
#define LEVEL2_TIME 180.0f

#define HERO_MAX_HP 100
#define HERO_ATTACK_DAMAGE 14
#define HERO_ATTACK_RANGE 60.0f
#define HERO_ATTACK_DURATION 0.18f
#define HERO_ATTACK_COOLDOWN 0.30f
#define HERO_BLOCK_DAMAGE_REDUCTION 0.75f // fraction of damage blocked while holding K
#define HERO_DODGE_DURATION 0.22f
#define HERO_DODGE_SPEED 620.0f
#define HERO_DODGE_COOLDOWN 0.55f
#define HERO_HIT_IFRAMES 0.4f // brief invulnerability after getting hit

#define ENEMY_WIDTH 54
#define ENEMY_HEIGHT 90
#define ENEMY_HITSTUN_DURATION 0.25f
#define ENEMY_ATTACK_ACTIVE_DURATION 0.15f

#define ENEMY_EASY_HP 40
#define ENEMY_EASY_DAMAGE 6
#define ENEMY_EASY_SPEED 110.0f
#define ENEMY_EASY_RANGE 55.0f
#define ENEMY_EASY_TELEGRAPH 0.65f
#define ENEMY_EASY_COOLDOWN 1.5f

#define ENEMY_MEDIUM_HP 65
#define ENEMY_MEDIUM_DAMAGE 10
#define ENEMY_MEDIUM_SPEED 155.0f
#define ENEMY_MEDIUM_RANGE 60.0f
#define ENEMY_MEDIUM_TELEGRAPH 0.45f
#define ENEMY_MEDIUM_COOLDOWN 1.1f

#define ENEMY_HARD_HP 95
#define ENEMY_HARD_DAMAGE 16
#define ENEMY_HARD_SPEED 195.0f
#define ENEMY_HARD_RANGE 65.0f
#define ENEMY_HARD_TELEGRAPH 0.30f
#define ENEMY_HARD_COOLDOWN 0.8f

// Fight arenas: hero can't run past an active enemy until it's defeated.
#define ARENA_WIDTH 700.0f
#define ARENA_ENEMY_OFFSET 380.0f
#define ARENA_CAMERA_LEAD 150.0f

#endif