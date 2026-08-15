#ifndef DIFFICULTY_H
#define DIFFICULTY_H

typedef enum Difficulty
{
    DIFF_EASY,
    DIFF_MEDIUM,
    DIFF_HARD,
    DIFF_COUNT
} Difficulty;

float DifficultySpeedMultiplier(Difficulty difficulty);
const char *DifficultyName(Difficulty difficulty);

#endif