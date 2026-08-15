#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <stdbool.h>
#include "config.h"

typedef struct LeaderboardEntry
{
    char name[LEADERBOARD_NAME_LEN];
    int coins;
    float time;
} LeaderboardEntry;

// Loads up to LEADERBOARD_SIZE entries from disk into `entries`, sets
// `count` to how many were found. If the file doesn't exist yet (first
// run), count is set to 0 - same "missing asset, don't crash" tolerance
// the rest of the game already uses for textures.
void LeaderboardLoad(LeaderboardEntry entries[LEADERBOARD_SIZE], int *count);

// Overwrites the save file with the current entries.
void LeaderboardSave(const LeaderboardEntry entries[LEADERBOARD_SIZE], int count);

// True if a run with this many coins/this time would place in the top
// LEADERBOARD_SIZE (either the board has room, or this run beats the
// current lowest-ranked entry).
bool LeaderboardQualifies(const LeaderboardEntry entries[LEADERBOARD_SIZE], int count, int coins, float time);

// Inserts newEntry in ranked position, shifting lower entries down and
// dropping the last one if the board was already full. Ranking: more
// coins is better; if coins tie, a lower time (faster finish) is better.
void LeaderboardInsert(LeaderboardEntry entries[LEADERBOARD_SIZE], int *count, LeaderboardEntry newEntry);

#endif