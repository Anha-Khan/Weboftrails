#ifndef HERO_H
#define HERO_H

#include "raylib.h"
#include <stdbool.h>

// ============================================================
// HERO.H / HERO.C
// Everything about our playable character lives here:
// - what data it carries (position, speed, sprite, etc.)
// - how it responds to input and gravity each frame
// - how it's drawn to the screen
//
// Keeping this separate from main.c means later, when we add
// web-swinging or combat, we just add more functions here
// instead of cluttering the main game loop.
// ============================================================

typedef struct Hero {
    Vector2 position;      // where the hero is on screen (top-left corner)
    Vector2 velocity;       // current speed in x and y
    int     width;
    int     height;
    bool    isGrounded;     // true when standing on the floor
    bool    facingRight;    // which way the sprite should face

    Texture2D idleTexture;
    Texture2D runTexture;
    Texture2D jumpTexture;
} Hero;

// Creates a hero at a starting position and loads its textures.
Hero HeroCreate(Vector2 startPosition);

// Frees textures when the hero is no longer needed (call at program end).
void HeroUnload(Hero *hero);

// Reads keyboard input and updates velocity/position/state for this frame.
// deltaTime = seconds since last frame (raylib gives us this).
void HeroUpdate(Hero *hero, float deltaTime);

// Draws the hero's current sprite at its current position.
void HeroDraw(const Hero *hero);

#endif // HERO_H
