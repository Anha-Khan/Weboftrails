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

#define LEVEL1_TIME 60.0f
#define LEVEL1_MIN_COINS 10
#define LEVEL1_LENGTH 8000.0f
#define LEVEL1_COIN_COUNT 20
#define LEVEL1_OBS_COUNT 15
#define LEVEL1_PIT_COUNT 6
#define LEVEL1_PIT_MIN_W 80.0f
#define LEVEL1_PIT_MAX_W 160.0f

#define DIFFICULTY_EASY_SPEED_MULT   0.8f
#define DIFFICULTY_MEDIUM_SPEED_MULT 1.0f
#define DIFFICULTY_HARD_SPEED_MULT   1.3f

#define LEADERBOARD_SIZE 5
#define LEADERBOARD_NAME_LEN 13 // 12 chars + null terminator
#define LEADERBOARD_FILE "leaderboard.txt"

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

#define OBS_STATIC_TALL_TEXTURE "assets/levels/obstacle_static_tall.png"
#define OBS_STATIC_LOW_TEXTURE "assets/levels/obstacle_static_low.png"
#define OBS_MOVING_TALL_TEXTURE "assets/levels/obstacle_moving_tall.png"
#define OBS_MOVING_LOW_TEXTURE "assets/levels/obstacle_moving_low.png"
#define DEBRIS_TEXTURE "assets/levels/debris_rock.png"


#define FONT_TITLE "assets/fonts/PressStart2P-Regular.ttf"

#define MENU_TITLE_TEXT "WEB OF TRAILS"
#define MENU_SUBTITLE_TEXT "Coins Await. Keep Running!"

#define MENU_CIRCLE_RADIUS 85.0f
#define MENU_CIRCLE_SPACING 260.0f
#define MENU_CIRCLE_SELECTED_SCALE 1.18f
#define MENU_CIRCLE_POP_OFFSET 14.0f
#define MENU_ANIM_SPEED 8.0f // higher = snappier slide/pop

#define MENU_BG_IMAGE "assets/menu/menu_background.jpg"

// Circle centers, measured against the source image. Approximate —
// nudge these if the highlight rings don't sit exactly on the circles.
#define MENU_EASY_CENTER_X 459.0f
#define MENU_EASY_CENTER_Y 236.0f
#define MENU_MEDIUM_CENTER_X 627.0f
#define MENU_MEDIUM_CENTER_Y 236.0f
#define MENU_HARD_CENTER_X 805.0f
#define MENU_HARD_CENTER_Y 236.0f
#define MENU_CIRCLE_RADIUS 94.0f

#endif