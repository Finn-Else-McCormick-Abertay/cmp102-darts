#pragma once

#include <ctime>
#include <random>

namespace random {
	void init();
	void init(unsigned int seed);

	bool randChance(double percentage);
	int randInt(int min, int max);
}