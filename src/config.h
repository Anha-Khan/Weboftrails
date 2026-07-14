#ifndef CONFIG_H
#define CONFIG_H

// ============================================================
// CONFIG.H
// This is the ONE file you edit to change how the game looks
// and feels without touching any game logic code.
//
// To swap your hero's look later: replace the PNG file at the
// path below with your own art (same filename), or change the
// filename here to point at a new file. Same for enemies/levels.
// ============================================================

// ---- Window ----
#define SCREEN_WIDTH   1280
#define SCREEN_HEIGHT  720
#define GAME_TITLE     "Web Hero"
#define TARGET_FPS     60

// ---- Asset paths (swap these files to change the look) ----
#define HERO_IDLE_TEXTURE   "assets/hero/hero_idle.png"
#define HERO_RUN_TEXTURE    "assets/hero/hero_run.png"
#define HERO_JUMP_TEXTURE   "assets/hero/hero_jump.png"

#define BG_FAR_TEXTURE      "assets/backgrounds/bg_far.png"
#define BG_MID_TEXTURE      "assets/backgrounds/bg_mid.png"
#define BG_NEAR_TEXTURE     "assets/backgrounds/bg_near.png"

// ---- Physics tuning (feel free to tweak these numbers) ----
#define GRAVITY            1800.0f   // pixels/sec^2, pulls hero down
#define MOVE_SPEED         420.0f    // pixels/sec, left-right speed
#define JUMP_VELOCITY      -700.0f   // negative = upward in raylib's y-axis
#define GROUND_Y           600.0f    // y-coordinate of the floor

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

#endif // CONFIG_H
