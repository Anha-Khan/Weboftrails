#ifndef CONFIG_H
#define CONFIG_H

// ---- Window ----
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define GAME_TITLE "Web of Trails"
#define TARGET_FPS 60

// ---- Asset paths ----
#define HERO_IDLE_TEXTURE "assets/hero/hero_idle.png"
#define HERO_RUN_TEXTURE "assets/hero/hero_run.png"
#define HERO_JUMP_TEXTURE "assets/hero/hero_jump.png"
#define HERO_DUCK_TEXTURE "assets/hero/hero_duck.png"
#define COIN_TEXTURE "assets/levels/coin.png"

// ---- Physics ----
#define GRAVITY 1800.0f
#define MOVE_SPEED 420.0f
#define JUMP_VELOCITY -700.0f
#define GROUND_Y 600.0f

// ---- Hero size ----
#define HERO_WIDTH 50
#define HERO_HEIGHT 80
#define HERO_DUCK_HEIGHT 40

// ---- Level 1 (edit these freely) ----
#define LEVEL1_TIME 60.0f
#define LEVEL1_MIN_COINS 10
#define LEVEL1_LENGTH 8000.0f
#define LEVEL1_COIN_COUNT 20
#define LEVEL1_OBS_COUNT 15

// ---- Coin ----
#define COIN_WIDTH 24
#define COIN_HEIGHT 24
#define COIN_FLOAT_Y (GROUND_Y - 80.0f)

// ---- Obstacles ----
#define OBS_TALL_WIDTH 40
#define OBS_TALL_HEIGHT 90
#define OBS_LOW_WIDTH 80
#define OBS_LOW_HEIGHT 35

#endif