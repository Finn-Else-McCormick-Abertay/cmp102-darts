#pragma once

#include <vector>
#include <string>

class Player {
	std::string name;

	double onTargetChance; double bullChance; double doubleChance; double tripleChance;

	int gamesPlayed; int gamesWon;
	int totalTurnsFromWinningGames;

public:
	Player(std::string name, double bullChance, double onTargetChance, double doubleChance, double tripleChance);
	Player(std::string name, double bullChance, double onTargetChance);

	std::string getName();
	double getOnTargetChance();
	double getBullChance();
	double getDoubleChance();
	double getTripleChance();

	int getNumWins();
	double getWinAverage();
	double getAverageTurnsToWin();

	void registerGame(bool won, int turns);
};

