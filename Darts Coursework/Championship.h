#pragma once

#include "Game.h"

#include <vector>
#include <map>

class Championship {
	struct ChampionshipPlayerInstance {
		Player* pointer;
		int sets; int games;

		ChampionshipPlayerInstance(Player* player);
	};

	std::vector<ChampionshipPlayerInstance> playerInstances;

	Game* game;

public:
	Championship(Game*, std::vector<Player*>& playerPointers);

	void runChampionship(int numSets = 13, int numGamesPerSet = 5);

	// Run x many times and output statistics
	void runRepeated(int repetitions);
};