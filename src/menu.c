#include "menu.h"
#include "config.h"

MenuState MenuCreate(void)
{
    MenuState menu = {0};
    menu.background = LoadTexture(MENU_BG_IMAGE);
    menu.selected = DIFF_MEDIUM;
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
    switch (d)
    {
        case DIFF_EASY:
            return (Vector2){MENU_EASY_CENTER_X, MENU_EASY_CENTER_Y};
        case DIFF_HARD:
            return (Vector2){MENU_HARD_CENTER_X, MENU_HARD_CENTER_Y};
        case DIFF_MEDIUM:
        default:
            return (Vector2){MENU_MEDIUM_CENTER_X, MENU_MEDIUM_CENTER_Y};
    }
}

bool MenuUpdate(MenuState *menu)
{
    // Keyboard fallback
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        menu->selected = (Difficulty)((menu->selected + DIFF_COUNT - 1) % DIFF_COUNT);
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        menu->selected = (Difficulty)((menu->selected + 1) % DIFF_COUNT);
    if (IsKeyPressed(KEY_ONE))
        menu->selected = DIFF_EASY;
    if (IsKeyPressed(KEY_TWO))
        menu->selected = DIFF_MEDIUM;
    if (IsKeyPressed(KEY_THREE))
        menu->selected = DIFF_HARD;

    // Clicking a circle selects that difficulty directly.
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
        // Stretched to fill the window exactly, so the circle coordinates
        // in config.h line up regardless of the source image's own resolution.
        DrawTexturePro(menu->background,
                       (Rectangle){0, 0, (float)menu->background.width, (float)menu->background.height},
                       (Rectangle){0, 0, (float)SCREEN_WIDTH, (float)SCREEN_HEIGHT},
                       (Vector2){0, 0}, 0, WHITE);
    }

    // The artwork itself doesn't show selection state, so a highlight
    // ring is drawn on top of whichever circle is currently selected.
    Vector2 c = CircleCenter(menu->selected);
    DrawCircleLines((int)c.x, (int)c.y, MENU_CIRCLE_RADIUS + 6, GOLD);
    DrawCircleLines((int)c.x, (int)c.y, MENU_CIRCLE_RADIUS + 8, GOLD);
    DrawCircleLines((int)c.x, (int)c.y, MENU_CIRCLE_RADIUS + 10, (Color){255, 215, 0, 150});

    const char *hint = "Click a circle or use ARROWS / 1-2-3, then ENTER to start";
    DrawText(hint, SCREEN_WIDTH / 2 - MeasureText(hint, 18) / 2,
             SCREEN_HEIGHT - 40, 18, (Color){50, 50, 50, 255});
}