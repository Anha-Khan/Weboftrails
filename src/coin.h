#ifndef COIN_H
#define COIN_H

#include "raylib.h"
#include "config.h"
#include <stdbool.h>

typedef struct Coin
{
    Vector2 position;
    bool collected;
    Texture2D texture;
} Coin;

Coin CoinCreate(Vector2 pos, Texture2D tex);
Rectangle CoinGetRect(const Coin *c);
void CoinDraw(const Coin *c, float camX);

#endif