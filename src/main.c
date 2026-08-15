#include "raylib.h"
#include "config.h"
#include "level1.h"
#include "difficulty.h"
#include "menu.h"

typedef enum GameStage
{
    STAGE_MENU,
    STAGE_PLAYING
} GameStage;

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
    SetTargetFPS(TARGET_FPS);

    GameStage stage = STAGE_MENU;
    MenuState menu = MenuCreate();
    Level1 level1 = {0};
    bool levelExists = false;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (stage == STAGE_MENU)
        {
            bool confirmed = MenuUpdate(&menu);
            if (confirmed)
            {
                if (levelExists)
                    Level1Unload(&level1);
                level1 = Level1Create(menu.selected);
                levelExists = true;
                stage = STAGE_PLAYING;
            }

            BeginDrawing();
            MenuDraw(&menu);
            EndDrawing();
            continue;
        }

        if (level1.state == L1_LOSE && IsKeyPressed(KEY_R))
        {
            Difficulty diff = level1.difficulty;
            Level1Unload(&level1);
            level1 = Level1Create(diff);
        }

        if (level1.state == L1_WIN && IsKeyPressed(KEY_ENTER))
            Level1AdvanceFromWin(&level1);

        if (level1.state == L1_LEADERBOARD && IsKeyPressed(KEY_ENTER))
        {
            Level1Unload(&level1);
            levelExists = false;
            stage = STAGE_MENU;
            continue;
        }

        Level1Update(&level1, dt);

        BeginDrawing();
        Level1Draw(&level1);
        EndDrawing();
    }

    if (levelExists)
        Level1Unload(&level1);
    MenuUnload(&menu);
    CloseWindow();
    return 0;
}