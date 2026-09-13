#include "leaderboard.h"
#include <stdio.h>
#include <string.h>

static bool IsBetter(const LeaderboardEntry *a, const LeaderboardEntry *b)
{
    return a->coins == b->coins ? a->time < b->time : a->coins > b->coins;
}

void LeaderboardLoad(LeaderboardEntry entries[LEADERBOARD_SIZE], int *count)
{
    *count = 0;
    FILE *f = fopen(LEADERBOARD_FILE, "r");
    if (!f)
        return;

    char name[LEADERBOARD_NAME_LEN];
    int coins;
    float time;
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
        return;
    for (int i = 0; i < count; i++)
        fprintf(f, "%s,%d,%.2f\n", entries[i].name, entries[i].coins, entries[i].time);
    fclose(f);
}

bool LeaderboardQualifies(const LeaderboardEntry entries[LEADERBOARD_SIZE], int count, int coins, float time)
{
    if (count < LEADERBOARD_SIZE)
        return true;
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
        return;

    int last = (*count < LEADERBOARD_SIZE) ? *count : LEADERBOARD_SIZE - 1;
    for (int i = last; i > insertAt; i--)
        entries[i] = entries[i - 1];

    entries[insertAt] = newEntry;
    if (*count < LEADERBOARD_SIZE)
        (*count)++;
}
