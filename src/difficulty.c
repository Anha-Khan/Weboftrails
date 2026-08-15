#include "difficulty.h"
#include "config.h"

float DifficultySpeedMultiplier(Difficulty difficulty)
{
    switch (difficulty)
    {
        case DIFF_EASY:
            return DIFFICULTY_EASY_SPEED_MULT;
        case DIFF_HARD:
            return DIFFICULTY_HARD_SPEED_MULT;
        case DIFF_MEDIUM:
        default:
            return DIFFICULTY_MEDIUM_SPEED_MULT;
    }
}

const char *DifficultyName(Difficulty difficulty)
{
    switch (difficulty)
    {
        case DIFF_EASY:
            return "EASY";
        case DIFF_HARD:
            return "HARD";
        case DIFF_MEDIUM:
        default:
            return "MEDIUM";
    }
}