#include "coin.h"
#include "config.h"

Coin CoinCreate(Vector2 pos, Texture2D tex)
{
    Coin c = {0};
    c.position = pos;
    c.collected = false;
    c.texture = tex;
    return c;
}

Rectangle CoinGetRect(const Coin *c)
{
    return (Rectangle){c->position.x, c->position.y,
                       COIN_WIDTH, COIN_HEIGHT};
}

void CoinDraw(const Coin *c, float camX)
{
    if (c->collected)
        return;

    float sx = c->position.x - camX;
    float sy = c->position.y;

    if (sx + COIN_WIDTH < 0 || sx > SCREEN_WIDTH)
        return;

    if (c->texture.id)
    {
        DrawTexture(c->texture, (int)sx, (int)sy, WHITE);
    }
    else
    {
        DrawCircle((int)(sx + COIN_WIDTH / 2),
                   (int)(sy + COIN_HEIGHT / 2),
                   COIN_WIDTH / 2, GOLD);
    }
}