# Web Hero

A 2.5D side-scrolling action game built in C with raylib.
Working title — rename freely.

## Project Structure

```
webhero/
├── Makefile              <- build instructions
├── src/
│   ├── main.c            <- entry point / game loop
│   ├── config.h          <- ALL settings + asset filenames (edit this freely)
│   ├── hero.h / hero.c   <- player character logic
├── assets/
│   ├── hero/             <- hero sprite images go here
│   ├── enemies/          <- enemy sprite images go here
│   ├── backgrounds/      <- parallax background layers go here
│   └── levels/           <- level data (added in a later stage)
└── build/                <- compiled game ends up here (auto-created)
```

## How to swap the hero's look later

Open `src/config.h`. Near the top you'll see:

```c
#define HERO_IDLE_TEXTURE   "assets/hero/hero_idle.png"
#define HERO_RUN_TEXTURE    "assets/hero/hero_run.png"
#define HERO_JUMP_TEXTURE   "assets/hero/hero_jump.png"
```

To use your own art: just save your PNG files into `assets/hero/` using
those exact filenames (or change the filenames in config.h to match
whatever you name your files). No other code needs to change.

If a file is missing, the game doesn't crash — it just draws a colored
rectangle instead, so you can keep playing/testing before art exists.

## Building and running (macOS)

You said you've installed raylib before, so first double check it's
actually there:

```bash
brew list raylib
```

If that says "Error: No such keg" or similar, install it with:

```bash
brew install raylib
```

Then, from inside the `webhero` folder:

```bash
make run
```

This compiles the game and launches it in one step. If you just want
to compile without running:

```bash
make
./build/webhero
```

To delete compiled files and start fresh:

```bash
make clean
```

## Controls (Stage 1)

- `A` / Left Arrow — move left
- `D` / Right Arrow — move right
- `SPACE` or `W` — jump

(Web-swinging, mouse-click combat, and level goals come in later stages.)

## What's implemented so far

- [x] Window + game loop
- [x] Hero movement (left/right, jump, gravity)
- [x] Swappable asset system with placeholder fallback
- [ ] Web-swinging mechanic
- [ ] Combat (mouse-click based)
- [ ] Parallax backgrounds
- [ ] Level 1 / 2 / 3 layouts + win condition
- [ ] Enemies
