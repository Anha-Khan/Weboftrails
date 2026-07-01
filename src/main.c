#include "raylib.h"
#include "config.h"
#include "level1.h"

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
    SetTargetFPS(TARGET_FPS);

    Level1 level1 = Level1Create();

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        // R to restart after loss
        if (level1.state == L1_LOSE && IsKeyPressed(KEY_R))
        {
            Level1Unload(&level1);
            level1 = Level1Create();
        }

        // ENTER after win (will go to Level 2 once built)
        if (level1.state == L1_WIN && IsKeyPressed(KEY_ENTER))
        {
            Level1Unload(&level1);
            level1 = Level1Create();
        }

        Level1Update(&level1, dt);

        BeginDrawing();
        Level1Draw(&level1);
        EndDrawing();
    }

    Level1Unload(&level1);
    CloseWindow();
    return 0;
}