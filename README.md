# Web Hero (Web of Trails)

A 2.5D side-scrolling action game built in C with raylib. Working title —
rename freely. Three people are building this together (Anha, Nazifa,
Chand) — see `CONTRIBUTING.md` before you start editing.

## Project structure

```
webhero/
├── Makefile              <- build instructions
├── src/
│   ├── main.c            <- entry point / game loop
│   ├── config.h          <- ALL settings + asset filenames (edit this freely)
│   ├── hero.h / hero.c   <- player character: movement, jump, duck, run animation
│   ├── obstacle.h/.c    <- JUMP/DUCK obstacles (some move up and down)
│   ├── debris.h / .c     <- falling-object hazard: warns, drops, has to be dodged
│   ├── coin.h / .c       <- collectible coins
│   └── level1.h / .c     <- level 1: layout, camera, win/lose, HUD
├── assets/
│   ├── hero/             <- hero sprite images go here
│   ├── enemies/          <- enemy sprite images go here (not used yet)
│   ├── backgrounds/      <- parallax background layers go here
│   └── levels/           <- level data / level-specific art (e.g. coin sprite)
└── build/                <- compiled game ends up here (auto-created)
```

## Controls

- `A` / Left Arrow — move left
- `D` / Right Arrow — move right
- `SPACE` or `W` — jump
- `S` / Down Arrow — duck
- `R` — restart after losing
- `ENTER` — continue after winning

## What's in Level 1

Run from the start to the "END" flag before the clock runs out, while
collecting at least `LEVEL1_MIN_COINS` coins along the way. Three kinds of
hazards are mixed along the path:

- **JUMP obstacles** (tall, brown) — jump over them. Some bob up and down
  (outlined in orange) instead of sitting still, so the timing changes.
- **DUCK obstacles** (low, purple) — duck under them. Same deal, some move.
- **Pits** — gaps in the ground with no floor. Fall in and it's game over,
  so jump across before you reach the edge.
- **Falling debris** — a red warning marker flashes on the ground to show
  where something is about to drop from above. You have a moment to move
  out of that spot sideways before it lands; get hit while it's falling
  and it's game over.

Bumping into a JUMP/DUCK obstacle just blocks your path (like a wall) —
it doesn't end the level, so you can back up and re-time your jump/duck.
The only instant-fail hazards are pits and falling debris.

Coins are worth grabbing but not required for every one — just hit the
minimum shown in the top-left HUD before reaching the end.

## Building and running

**Mac/Linux:** see `GETTING_STARTED.md`. Short version, once raylib is
installed (`brew install raylib`):

```bash
make run
```

**Windows (Visual Studio):** see `GETTING_STARTED_WINDOWS.md`.

Other useful commands:

```bash
make        # compile only
make clean  # delete compiled files and start fresh
```

## Swapping the hero's look (or any asset)

Open `src/config.h` — near the top you'll see filenames like:

```c
#define HERO_IDLE_TEXTURE   "assets/hero/hero_idle.png"
#define HERO_RUN_TEXTURE_1  "assets/hero/hero_run1.png"
#define HERO_RUN_TEXTURE_2  "assets/hero/hero_run2.png"
#define HERO_RUN_TEXTURE_3  "assets/hero/hero_run3.png"
#define HERO_JUMP_TEXTURE   "assets/hero/hero_jump.png"
#define HERO_DUCK_TEXTURE   "assets/hero/hero_duck.png"
```

Save your own PNGs into `assets/hero/` using those exact filenames (or
change the filenames in `config.h` to match whatever you name your
files). No other code needs to change. If a file is missing, the game
doesn't crash — it draws a colored placeholder shape instead, so you can
keep playing/testing before the art exists.

## What's in Level 2

After clearing Level 1 you get a short "Level 2 unlocked" screen, then
drop into a combat stage: run along a path and fight three enemies in a
row (easy, then medium, then hard). Each fight locks the camera in place
until that enemy is down, then you keep running to the next one.

Combat controls:
- `J` — attack (short swing in front of you, has a brief cooldown)
- `K` — hold to block (cuts incoming damage, can't attack/dodge at the
  same time)
- `L` — dodge (a quick dash in the direction you're facing, with a
  short window of invulnerability — this is the escape option when an
  enemy's attack is telegraphed)

Enemies wind up (flash red) before swinging — that flash is your cue to
dodge, block, or just back out of range. Your HP bar is above your head;
run out and it's game over (`R` to retry). Beat all three and you get a
"Level 2 complete" screen (Level 3 isn't built yet).

## Status

**Level 1**
- [x] Running, jumping, ducking, gravity
- [x] Camera that scrolls with the hero
- [x] JUMP/DUCK obstacles, including ones that move
- [x] Pits you can fall into
- [x] Falling debris hazard (telegraphed, dodge sideways)
- [x] Coins, timer, win/lose states, restart
- [x] Run-cycle animation
- [ ] `hero_jump.png` / `hero_duck.png` art (currently placeholder boxes)
- [ ] `assets/levels/coin.png` art (currently a placeholder gold circle)

**Level 1 -> Level 2 transition**
- [x] Congratulations/unlock screen after winning Level 1

**Level 2**
- [x] HP bar above the hero's head
- [x] Attack / block / dodge combat
- [x] Three enemies (easy/medium/hard) fought one at a time, in sequence
- [x] Per-enemy HP bar + telegraphed attacks
- [x] Win/lose states, restart
- [ ] Enemy sprite art (currently colored placeholder boxes)
- [ ] Web-swinging mechanic
- [ ] Level 3

See `CONTRIBUTING.md` for how we merge work between branches, and open a
pull request into `main` once a feature is working — nobody edits `main`
directly.
