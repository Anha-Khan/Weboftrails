# Web of Trails

Web of Trails is a 2D side-scrolling runner built in C with
[raylib](https://www.raylib.com/). Choose a difficulty, collect enough coins,
avoid the hazards, and reach the end before time runs out.

## Screenshots

### Difficulty menu

![Web of Trails difficulty menu](docs/screenshots/menu.png)

### Gameplay

![Web of Trails gameplay](docs/screenshots/gameplay.png)

### Jumping

Press `W` or Space to jump. The hero uses a separate jump pose while airborne.

![Hero performing a jump](docs/screenshots/jump.png)

### Avoiding pits

Start the jump before reaching the edge and clear the entire opening before
landing.

![Hero jumping across a pit](docs/screenshots/dodge-pit.png)

### Avoiding a ground obstacle

Jump high enough for the hero's full collision area to pass over the obstacle.

![Hero jumping over a ground obstacle](docs/screenshots/dodge-static-obstacle.png)

### Avoiding a moving obstacle

Watch its vertical movement and jump when there is a safe opening.

![Hero avoiding a moving obstacle](docs/screenshots/dodge-moving-obstacle.png)

### Avoiding falling debris

Move away from the warning position before the debris reaches the hero.

![Hero moving away from falling debris](docs/screenshots/dodge-falling-debris.png)

### Leaderboard

After a successful run, qualifying scores appear in the local top five.

![Web of Trails top-five leaderboard](docs/screenshots/leaderboard.png)

## How to play

1. Select **Easy** or **Hard** from the opening menu.
2. Collect at least **10 coins**.
3. Jump over pits and obstacles, and avoid falling debris.
4. Reach the end of the level before the **60-second timer** expires.

Running into a normal obstacle blocks the hero but does not end the game.
Falling into a pit, being hit by falling debris, or running out of time causes
a game over.

## Controls

| Action | Keyboard control |
| --- | --- |
| Move left | `A` or Left Arrow |
| Move right | `D` or Right Arrow |
| Jump | `W` or Space |
| Select difficulty | Left/Right Arrow, `A`/`D`, or `1`/`2` |
| Start the game | Enter |
| Restart after losing | `R` |
| Continue after winning | Enter |

The difficulty options can also be selected with the mouse.

## Difficulty modes

- **Easy** uses a slower game speed and is best for learning the level.
- **Hard** increases the speed of the level and its hazards.

## Hazards and collectibles

| Item | Behaviour |
| --- | --- |
| Coin | Adds one coin to the player's score |
| Obstacle | Blocks the hero; jump over it to continue |
| Moving obstacle | Moves vertically and must be timed carefully |
| Pit | Ends the run after the hero falls into it |
| Falling debris | Shows a warning before falling; a direct hit ends the run |

## Game architecture

The game uses a small module-based architecture. `main.c` owns the application
loop and switches between the menu and gameplay. `level1.c` coordinates the
level modules and decides when the player wins or loses.

```mermaid
flowchart TD
    Main["main.c<br>Application loop and screen switching"]
    Menu["menu.c<br>Difficulty selection"]
    Level["level1.c<br>Level state, camera, HUD and collisions"]
    Difficulty["difficulty.c<br>Easy and Hard speed settings"]
    Hero["hero.c<br>Movement, jumping and animation"]
    Coins["coin.c<br>Collectibles"]
    Obstacles["obstacle.c<br>Static and moving obstacles"]
    Debris["debris.c<br>Falling hazards"]
    Board["leaderboard.c<br>Local top-five scores"]
    Config["config.h<br>Constants and asset paths"]

    Main --> Menu
    Main --> Level
    Menu --> Difficulty
    Level --> Difficulty
    Level --> Hero
    Level --> Coins
    Level --> Obstacles
    Level --> Debris
    Level --> Board
    Config -.-> Main
    Config -.-> Menu
    Config -.-> Level
```

Each gameplay frame follows the same simple flow:

```text
Read input -> update the world -> check collisions and game state -> draw
```

## Project structure

```text
.
├── assets/                 Images, sprites, fonts, and level art
├── docs/screenshots/       Screenshots used in this README
├── src/
│   ├── main.c              Entry point and main loop
│   ├── config.h            Game settings and asset paths
│   ├── menu.c              Difficulty menu
│   ├── difficulty.c        Difficulty values
│   ├── level1.c            Level rules, layout, camera, HUD, and states
│   ├── hero.c              Hero controls and animation
│   ├── coin.c              Coin logic
│   ├── obstacle.c          Obstacle logic
│   ├── debris.c            Falling-debris logic
│   └── leaderboard.c       Local leaderboard logic
├── Makefile                Build commands
└── leaderboard.txt         Local scores, created when needed
```

Header files in `src/` define the public interface for each matching `.c`
module.

## Build and run

### Requirements

- A C99-compatible compiler
- GNU Make
- raylib 5.x

### macOS

Install raylib with Homebrew, then run the game from the project directory:

```bash
brew install raylib
make run
```

See [GETTING_STARTED.md](GETTING_STARTED.md) for more macOS and Linux setup
details.

### Windows

Follow [GETTING_STARTED_WINDOWS.md](GETTING_STARTED_WINDOWS.md) for the Visual
Studio setup.

### Useful commands

```bash
make        # Build the game at build/webhero
make run    # Build and start the game
make clean  # Remove compiled build files
```

Always start the game from the repository root so its relative asset paths can
be found.

## Leaderboard

After completing the level, enter a player name to save the result. The game
stores the best five scores locally in `leaderboard.txt`. Scores are ranked by
coins collected, with a faster completion time used as the tiebreaker.

The current leaderboard is local to one computer. A shared online leaderboard
would require a hosted database and server-side API.

## Changing settings or artwork

Most gameplay values and asset paths are collected in `src/config.h`. Use that
file to adjust values such as the time limit, required coins, movement speed,
screen size, or image filenames.

Artwork lives under `assets/`. When replacing an image, either keep its current
filename or update the corresponding path in `src/config.h`.

## Current scope

- One playable side-scrolling level
- Easy and Hard difficulty modes
- Coins, pits, obstacles, and falling debris
- Win, loss, restart, and countdown states
- Local top-five leaderboard
- Native desktop build using raylib

See [CONTRIBUTING.md](CONTRIBUTING.md) before making collaborative changes.
