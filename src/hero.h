#ifndef HERO_H
#define HERO_H

#include "raylib.h"
#include <stdbool.h>

typedef struct Hero
{
    Vector2 position;
    Vector2 velocity;
    int width;
    int height;
    bool isGrounded;
    bool facingRight;
    bool isDucking;
    Texture2D idleTexture;
    Texture2D runTexture;
    Texture2D jumpTexture;
    Texture2D duckTexture;
} Hero;

Hero HeroCreate(Vector2 startPosition);
void HeroUnload(Hero *hero);
void HeroUpdate(Hero *hero, float deltaTime, float cameraX);
void HeroDraw(const Hero *hero, float cameraX);
Rectangle HeroGetRect(const Hero *hero);

#endif