#include "difficulty.h"
#include "config.h"

float DifficultySpeedMultiplier(Difficulty difficulty)
{
    return difficulty == DIFF_HARD ? DIFFICULTY_HARD_SPEED_MULT
                                   : DIFFICULTY_EASY_SPEED_MULT;
}

const char *DifficultyName(Difficulty difficulty)
{
    return difficulty == DIFF_HARD ? "HARD" : "EASY";
}
