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
    InitAudioDevice();
    SetTargetFPS(TARGET_FPS);

    GameStage stage = STAGE_MENU;
    MenuState menu = MenuCreate();
    PlayMusicStream(menu.music);

    Level1 level1 = {0};
    bool levelExists = false;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (stage == STAGE_MENU)
        {
            UpdateMusicStream(menu.music); // required every frame for streamed audio to keep buffering

            bool confirmed = MenuUpdate(&menu);
            if (confirmed)
            {
                StopMusicStream(menu.music);
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
            PlayMusicStream(menu.music); // restart the menu track when returning to it
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
    CloseAudioDevice();
    CloseWindow();
    return 0;
}