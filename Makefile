CC  = gcc
SRC = src/main.c src/hero.c src/coin.c src/obstacle.c src/debris.c src/enemy.c src/level1.c src/level2.c

ifeq ($(OS),Windows_NT)
    OUT     = build/webhero.exe
    CFLAGS  = -Wall -std=c99 -I/ucrt64/include
    LDFLAGS = -L/ucrt64/lib -lraylib -lopengl32 -lgdi32 -lwinmm
    MKDIR   = mkdir -p build
    RUN     = ./$(OUT)
    CLEAN   = rm -rf build
else
    OUT     = build/webhero
    BREW_PREFIX := $(shell brew --prefix raylib 2>/dev/null)
    CFLAGS  = -Wall -std=c99 -I$(BREW_PREFIX)/include
    LDFLAGS = -L$(BREW_PREFIX)/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL
    MKDIR   = mkdir -p build
    RUN     = ./$(OUT)
    CLEAN   = rm -rf build
endif

all: $(OUT)

$(OUT): $(SRC)
	$(MKDIR)
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

run: all
	$(RUN)

clean:
	$(CLEAN)

.PHONY: all run clean