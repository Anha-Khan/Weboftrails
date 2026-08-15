#include "leaderboard.h"
#include <stdio.h>
#include <string.h>

// Higher coins ranks first; if coins are equal, the lower time ranks
// first (finishing the level faster is the tiebreaker).
static bool IsBetter(const LeaderboardEntry *a, const LeaderboardEntry *b)
{
    if (a->coins != b->coins)
        return a->coins > b->coins;
    return a->time < b->time;
}

void LeaderboardLoad(LeaderboardEntry entries[LEADERBOARD_SIZE], int *count)
{
    *count = 0;
    FILE *f = fopen(LEADERBOARD_FILE, "r");
    if (!f)
        return; // no save file yet - empty board

    char name[LEADERBOARD_NAME_LEN];
    int coins;
    float time;
    // Field width "12" must match LEADERBOARD_NAME_LEN - 1 (config.h) -
    // scanf field widths can't take a macro directly, so keep these in sync.
    while (*count < LEADERBOARD_SIZE &&
           fscanf(f, "%12[^,],%d,%f\n", name, &coins, &time) == 3)
    {
        strncpy(entries[*count].name, name, LEADERBOARD_NAME_LEN - 1);
        entries[*count].name[LEADERBOARD_NAME_LEN - 1] = '\0';
        entries[*count].coins = coins;
        entries[*count].time = time;
        (*count)++;
    }
    fclose(f);
}

void LeaderboardSave(const LeaderboardEntry entries[LEADERBOARD_SIZE], int count)
{
    FILE *f = fopen(LEADERBOARD_FILE, "w");
    if (!f)
        return; // can't write - not fatal, just means this run's score isn't persisted
    for (int i = 0; i < count; i++)
        fprintf(f, "%s,%d,%.2f\n", entries[i].name, entries[i].coins, entries[i].time);
    fclose(f);
}

bool LeaderboardQualifies(const LeaderboardEntry entries[LEADERBOARD_SIZE], int count, int coins, float time)
{
    if (count < LEADERBOARD_SIZE)
        return true; // board isn't full yet - always room
    LeaderboardEntry candidate = {"", coins, time};
    return IsBetter(&candidate, &entries[count - 1]);
}

void LeaderboardInsert(LeaderboardEntry entries[LEADERBOARD_SIZE], int *count, LeaderboardEntry newEntry)
{
    int insertAt = *count;
    for (int i = 0; i < *count; i++)
    {
        if (IsBetter(&newEntry, &entries[i]))
        {
            insertAt = i;
            break;
        }
    }
    if (insertAt >= LEADERBOARD_SIZE)
        return; // doesn't make the cut

    int last = (*count < LEADERBOARD_SIZE) ? *count : LEADERBOARD_SIZE - 1;
    for (int i = last; i > insertAt; i--)
        entries[i] = entries[i - 1];

    entries[insertAt] = newEntry;
    if (*count < LEADERBOARD_SIZE)
        (*count)++;
}