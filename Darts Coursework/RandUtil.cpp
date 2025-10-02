#include "RandUtil.h"

void random::init() { std::srand((unsigned int) time(NULL)); }

void random::init(unsigned int seed) { std::srand(seed); }

bool random::randChance(double percentage) { return (rand() % 100) < percentage; }

int random::randInt(int min, int max) { return min + (int)std::round(rand() % (max - min + 1)); }