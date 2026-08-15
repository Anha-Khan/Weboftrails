#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include "difficulty.h"
#include <stdbool.h>

typedef struct MenuState
{
    Texture2D background;
    Difficulty selected;
    Music music;
} MenuState;

MenuState MenuCreate(void);
void MenuUnload(MenuState *menu);

// Handles mouse clicks on the circles and keyboard fallback (arrows/1-2-3).
// Returns true the frame ENTER is pressed - caller should start the game then.
bool MenuUpdate(MenuState *menu);

void MenuDraw(const MenuState *menu);

#endif