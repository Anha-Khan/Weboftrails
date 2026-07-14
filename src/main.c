#include "raylib.h"
#include "config.h"
#include "level1.h"
#include "level2.h"

typedef enum GameStage
{
    STAGE_LEVEL1,
    STAGE_TRANSITION, // congrats screen between level 1 and level 2
    STAGE_LEVEL2
} GameStage;

static void DrawTransitionScreen(void)
{
    ClearBackground((Color){12, 12, 22, 255});

    const char *title = "LEVEL 1 COMPLETE!";
    const char *l1 = "You've successfully unlocked Level 2.";
    const char *l2 = "Let's fight off your enemies -";
    const char *l3 = "one step closer to becoming the hero.";
    const char *prompt = "Press ENTER to begin Level 2";

    DrawText(title, SCREEN_WIDTH / 2 - MeasureText(title, 52) / 2,
             SCREEN_HEIGHT / 2 - 160, 52, GOLD);
    DrawText(l1, SCREEN_WIDTH / 2 - MeasureText(l1, 28) / 2,
             SCREEN_HEIGHT / 2 - 80, 28, WHITE);
    DrawText(l2, SCREEN_WIDTH / 2 - MeasureText(l2, 26) / 2,
             SCREEN_HEIGHT / 2 - 30, 26, LIGHTGRAY);
    DrawText(l3, SCREEN_WIDTH / 2 - MeasureText(l3, 26) / 2,
             SCREEN_HEIGHT / 2, 26, LIGHTGRAY);
    DrawText(prompt, SCREEN_WIDTH / 2 - MeasureText(prompt, 26) / 2,
             SCREEN_HEIGHT / 2 + 80, 26, YELLOW);
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE);
    SetTargetFPS(TARGET_FPS);

    GameStage stage = STAGE_LEVEL1;
    Level1 level1 = Level1Create();
    Level2 level2 = {0};
    bool level2Created = false;

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();

        if (stage == STAGE_LEVEL1)
        {
            // R to restart after loss
            if (level1.state == L1_LOSE && IsKeyPressed(KEY_R))
            {
                Level1Unload(&level1);
                level1 = Level1Create();
            }

            // ENTER after win moves on to the Level 2 unlock screen
            if (level1.state == L1_WIN && IsKeyPressed(KEY_ENTER))
            {
                stage = STAGE_TRANSITION;
            }

            Level1Update(&level1, dt);
        }
        else if (stage == STAGE_TRANSITION)
        {
            if (IsKeyPressed(KEY_ENTER))
            {
                if (level2Created)
                    Level2Unload(&level2);
                level2 = Level2Create();
                level2Created = true;
                stage = STAGE_LEVEL2;
            }
        }
        else if (stage == STAGE_LEVEL2)
        {
            if (level2.state == L2_LOSE && IsKeyPressed(KEY_R))
            {
                Level2Unload(&level2);
                level2 = Level2Create();
            }

            if (level2.state == L2_WIN && IsKeyPressed(KEY_ENTER))
            {
                Level2Unload(&level2);
                level2 = Level2Create();
            }

            Level2Update(&level2, dt);
        }

        BeginDrawing();
        if (stage == STAGE_LEVEL1)
            Level1Draw(&level1);
        else if (stage == STAGE_TRANSITION)
            DrawTransitionScreen();
        else if (stage == STAGE_LEVEL2)
            Level2Draw(&level2);
        EndDrawing();
    }

    Level1Unload(&level1);
    if (level2Created)
        Level2Unload(&level2);
    CloseWindow();
    return 0;
}
