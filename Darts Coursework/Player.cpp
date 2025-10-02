#include "Player.h"

Player::Player(std::string name, double onTargetChance, double bullChance, double doubleChance, double tripleChance) {
	this->name = name;
	this->onTargetChance = onTargetChance; this->bullChance = bullChance;
	this->doubleChance = doubleChance; this->tripleChance = tripleChance;
	gamesPlayed = 0; gamesWon = 0;
	totalTurnsFromWinningGames = 0;
}

Player::Player(std::string name, double onTargetChance, double bullChance) : Player(name,onTargetChance,bullChance,bullChance,bullChance) {}

std::string Player::getName() { return name; }
double Player::getOnTargetChance() { return onTargetChance; }
double Player::getBullChance() { return bullChance; }
double Player::getDoubleChance() { return doubleChance; }
double Player::getTripleChance() { return tripleChance; }

int Player::getNumWins() { return gamesWon; };
double Player::getWinAverage() { return ((double)gamesWon / (double)gamesPlayed) * 100; }
double Player::getAverageTurnsToWin() { return (double)totalTurnsFromWinningGames / (double)gamesWon; }

void Player::registerGame(bool won, int turns) {
	gamesPlayed++;
	if (won) {
		gamesWon++;
		totalTurnsFromWinningGames += turns;
	}
}
