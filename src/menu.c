#include "menu.h"
#include "config.h"

MenuState MenuCreate(void)
{
    MenuState menu = {0};
    menu.background = LoadTexture(MENU_BG_IMAGE);
    menu.selected = DIFF_EASY;
    menu.music = LoadMusicStream(MENU_MUSIC);
    menu.music.looping = true;
    return menu;
}

void MenuUnload(MenuState *menu)
{
    if (menu->background.id)
        UnloadTexture(menu->background);
        UnloadMusicStream(menu->music);
}

static Vector2 CircleCenter(Difficulty d)
{
    float x = (d == DIFF_HARD) ? MENU_HARD_CENTER_X : MENU_EASY_CENTER_X;
    return (Vector2){x, MENU_OPTION_CENTER_Y};
}

static void DrawCenteredText(const char *text, int y, int size, Color color)
{
    DrawText(text, SCREEN_WIDTH / 2 - MeasureText(text, size) / 2, y, size, color);
}

bool MenuUpdate(MenuState *menu)
{
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        menu->selected = (Difficulty)((menu->selected + DIFF_COUNT - 1) % DIFF_COUNT);
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        menu->selected = (Difficulty)((menu->selected + 1) % DIFF_COUNT);
    if (IsKeyPressed(KEY_ONE))
        menu->selected = DIFF_EASY;
    if (IsKeyPressed(KEY_TWO))
        menu->selected = DIFF_HARD;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        for (int i = 0; i < DIFF_COUNT; i++)
        {
            Vector2 c = CircleCenter((Difficulty)i);
            if (CheckCollisionPointCircle(mouse, c, MENU_CIRCLE_RADIUS))
            {
                menu->selected = (Difficulty)i;
                break;
            }
        }
    }

    return IsKeyPressed(KEY_ENTER);
}

void MenuDraw(const MenuState *menu)
{
    ClearBackground(BLACK);
    if (menu->background.id)
    {
        DrawTexturePro(menu->background,
                       (Rectangle){0, 0, (float)menu->background.width, (float)menu->background.height},
                       (Rectangle){0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT},
                       (Vector2){0, 0}, 0, WHITE);
    }

    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){4, 24, 36, 75});

    DrawCenteredText(MENU_TITLE_TEXT, 85, 54, RAYWHITE);
    DrawCenteredText(MENU_SUBTITLE_TEXT, 150, 24, (Color){222, 239, 230, 255});

    for (int i = 0; i < DIFF_COUNT; i++)
    {
        Difficulty option = (Difficulty)i;
        Vector2 c = CircleCenter(option);
        bool selected = option == menu->selected;
        Color fill = selected ? (Color){250, 244, 220, 245} : (Color){10, 45, 62, 220};
        Color label = selected ? (Color){67, 45, 19, 255} : RAYWHITE;

        DrawCircleV(c, MENU_CIRCLE_RADIUS, fill);
        DrawCircleLines((int)c.x, (int)c.y, MENU_CIRCLE_RADIUS, selected ? GOLD : RAYWHITE);
        if (selected)
            DrawCircleLines((int)c.x, (int)c.y, MENU_CIRCLE_RADIUS + 7, GOLD);

        const char *name = DifficultyName(option);
        DrawText(name, (int)c.x - MeasureText(name, 34) / 2,
                 (int)c.y - 18, 34, label);
    }

    const char *hint = "Click a choice or use ARROWS / 1-2, then ENTER to start";
    DrawCenteredText(hint, SCREEN_HEIGHT - 40, 18, RAYWHITE);
}
