#include "raylib.h"
#include "config.h"
#include "hero.h"

// ============================================================
// MAIN.C
// This is the entry point. Its job is small on purpose:
//   1. Open the window
//   2. Create our game objects (right now, just the hero)
//   3. Loop: read input -> update game state -> draw frame
//   4. Clean up and close
//
// As we add web-swinging, enemies, and levels, each of those
// gets its own .h/.c pair (like hero.h/hero.c), and main.c just
// calls their Update/Draw functions here. Main.c itself should
// stay short and readable.
// ============================================================

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
    SetTargetFPS(TARGET_FPS);

    Hero hero = HeroCreate((Vector2){ 100, GROUND_Y - HERO_HEIGHT });

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // ---- UPDATE ----
        HeroUpdate(&hero, deltaTime);

        // ---- DRAW ----
        BeginDrawing();
            ClearBackground(SKYBLUE);

            // Temporary ground line until we have real level art.
            DrawRectangle(0, (int)GROUND_Y, SCREEN_WIDTH,
                           SCREEN_HEIGHT - (int)GROUND_Y, DARKGREEN);

            HeroDraw(&hero);

            DrawText("A/D or Arrow Keys = move   |   SPACE/W = jump",
                     20, 20, 20, DARKGRAY);
            DrawFPS(SCREEN_WIDTH - 90, 10);
        EndDrawing();
    }

    HeroUnload(&hero);
    CloseWindow();
    return 0;
}
