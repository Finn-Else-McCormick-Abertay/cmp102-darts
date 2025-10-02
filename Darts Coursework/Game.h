#pragma once

#include <vector>

#include "RandUtil.h"
#include "IOUtil.h"
#include "Player.h"

class Board {
	const int order[20] = { 20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5 };

	int getIndexOfValue(int val);
public:
	int getRandom();

	int getLeftOf(int val);
	int getRightOf(int val);
};



class Game {
protected:
	struct PlayerInstance {
		Player* pointer;
		int score;
		int turn;

		PlayerInstance(Player* pointer, int startingScore);
	};

	virtual bool playTurn(PlayerInstance& playerInst, bool output = false) = 0;

	Board board;
	int startingScore;

public:
	Game(int startingScore);

	Player& play(std::vector<Player*>& players, bool output = false);
};



class Darts301 : public Game {
	bool playTurn(PlayerInstance& playerInst, bool output = false);
public:
	Darts301();
};

class Darts501 : public Game {
	bool playTurn(PlayerInstance& playerInst, bool output = false);
public:
	Darts501();
};
