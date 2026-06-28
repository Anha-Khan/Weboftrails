# ============================================================
# Makefile for Web Hero
# Built for macOS with raylib installed via Homebrew.
#
# Usage:
#   make        -> builds the game into build/webhero
#   make run    -> builds AND runs it
#   make clean  -> deletes build files
# ============================================================

CC = gcc
SRC = src/main.c src/hero.c
OUT = build/webhero

# Homebrew installs raylib here on Apple Silicon Macs (M1/M2/M3...).
# If you're on an Intel Mac, Homebrew uses /usr/local instead - the
# line below handles both automatically.
BREW_PREFIX := $(shell brew --prefix raylib 2>/dev/null)

CFLAGS = -Wall -std=c99 -I$(BREW_PREFIX)/include
LDFLAGS = -L$(BREW_PREFIX)/lib -lraylib -framework IOKit -framework Cocoa -framework OpenGL

all: $(OUT)

$(OUT): $(SRC)
	mkdir -p build
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

run: all
	./$(OUT)

clean:
	rm -rf build

.PHONY: all run clean
