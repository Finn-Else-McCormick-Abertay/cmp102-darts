#include "Game.h"

int Board::getRandom() {
	int index = random::randInt(0, 19);
	return order[index];
}

int Board::getIndexOfValue(int val) {
	for (const int& i : order) {
		if (i == val) { return i; }
	}
	return -1;
}

int Board::getLeftOf(int val) {
	int index = getIndexOfValue(val);
	if (index == -1) { return 0; }
	index -= 1;
	if (index < 0) { index += 20; }
	return order[index];
}

int Board::getRightOf(int val) {
	int index = getIndexOfValue(val);
	if (index == -1) { return 0; }
	index += 1;
	if (index >= 20) { index -= 20; }
	return order[index];
}



Game::Game(int startingScore) { this->startingScore = startingScore; }

Game::PlayerInstance::PlayerInstance(Player* player, int startingScore) {
	pointer = player;
	score = startingScore;
	turn = 0;
}

Player& Game::play(std::vector<Player*>& players, bool output) {
	std::vector<PlayerInstance> playerInstances;
	for (Player* player : players) {
		PlayerInstance instance(player, startingScore);
		playerInstances.push_back(instance);
	}

	Player* winner = NULL;
	if (output) { std::cin.ignore(); } // Have to clear input buffer before first using cin.get()

	bool gameIsOver = false;
	do {
		if (output) { println("-------\n"); }
		for (PlayerInstance& playerInst : playerInstances) {
			gameIsOver = playTurn(playerInst, output);
			if (gameIsOver) { winner = playerInst.pointer; break; } // If over break from for loop, skipping later players' goes
		}
		if (output) { if (gameIsOver) { println(winner->getName(), " is the winner!"); } else { std::cin.get(); } }
	} while (!gameIsOver);

	for (PlayerInstance& playerInst : playerInstances) { playerInst.pointer->registerGame(playerInst.score <= 0, playerInst.turn-1); } // Register game with players

	return *winner;
}



// --- 301 ---

Darts301::Darts301() : Game(301) {}

bool Darts301::playTurn(PlayerInstance& playerInst, bool output) {
	int goal = 50;
	if (playerInst.score > 50 && playerInst.score < 100) { goal = std::min(playerInst.score - 50, 20); }

	int scoreReduction = 0;
	if (goal == 50) {
		bool hitBull = random::randChance(playerInst.pointer->getBullChance());
		if (hitBull) { scoreReduction = 50; }
		else { scoreReduction = board.getRandom(); }
	}
	else {
		bool hitIntended = random::randChance(playerInst.pointer->getOnTargetChance());
		if (hitIntended) { scoreReduction = goal; }
		else { scoreReduction = random::randChance(50) ? board.getLeftOf(goal) : board.getRightOf(goal); }
	}

	bool canReduce = playerInst.score - scoreReduction == 0 || playerInst.score - scoreReduction >= 50;
	if (canReduce) { playerInst.score -= scoreReduction; }

	if (output) {
		println(playerInst.pointer->getName(), " turn ", playerInst.turn, ":");
		print("Attempted to hit "); (goal == 50) ? print("bull") : print(goal);
		if (goal == scoreReduction) { print(" succesfully"); } else { print(", hit "); (scoreReduction == 50) ? print("bull") : print(scoreReduction); } println(".");
		if (!canReduce) { println("Hit was invalid."); }
		println("Score: ", playerInst.score);
		println();
	}

	playerInst.turn++;
	return playerInst.score == 0;
}



// --- 501 ---

Darts501::Darts501() : Game(501) {}

bool Darts501::playTurn(PlayerInstance& playerInst, bool output) {
	static auto isValid = [](int goal) { return (goal <= 20 || goal == 25 || goal == 50 || (goal <= 40 && goal % 2 == 0) || (goal <= 60 && goal % 3 == 0)); };
	int scoreReduction = 0;

	if (output) { println(playerInst.pointer->getName(), " turn ", playerInst.turn, ":"); }

	int finalWorkingScore = 0;
	for (int i = 0; i < 3; i++) { // For each throw
		int workingScore = playerInst.score - scoreReduction; finalWorkingScore = workingScore;
		int goal = 1;
		if (workingScore > 50 * (3 - i) && workingScore > 60) { goal = 50; } // If out of range to finish, just try to get the score down
		else if ((i == 2) && (workingScore == 50 || (workingScore >= 40 && workingScore % 2 == 0))) { goal = workingScore; } // If can make final throw, try
		else if (workingScore <= 60 && (i != 2)) {
			for (int desiredFinalScore = 40; desiredFinalScore > (i == 0 ? 2 : 0); desiredFinalScore -= 2) {
				int possibleGoal = workingScore - desiredFinalScore;
				if (possibleGoal > 0 && isValid(possibleGoal)) { goal = possibleGoal; break; }
			}
		}
		else {
			if (workingScore >= 20) { goal = 20; } // Focus on 20 and 16 towards the end of rounds
			else if (workingScore >= 16) { goal = 16; }

			for (int possibleGoal = workingScore; possibleGoal > 1; possibleGoal--) {
				if (isValid(possibleGoal)) { goal = possibleGoal; break; }
			}
		}

		int thisDartScoreReduction = 0;

		// Shoot for outer bull
		if (goal == 25) {
			bool hitIntended = random::randChance(playerInst.pointer->getBullChance());
			if (hitIntended) { thisDartScoreReduction = goal; }
			else { thisDartScoreReduction = random::randChance(10) ? 50 : board.getRandom(); }
		}
		// Shoot for inner bull
		else if (goal == 50) {
			bool hitIntended = random::randChance(playerInst.pointer->getBullChance());
			if (hitIntended) { thisDartScoreReduction = goal; }
			else { thisDartScoreReduction = random::randChance(10) ? 25 : board.getRandom(); }
		}
		// Shoot for single
		else if (goal <= 20 && (i != 2)) {
			bool hitIntended = random::randChance(playerInst.pointer->getOnTargetChance());

			bool accidentalDouble = random::randChance(3); bool accidentalTriple = random::randChance(1);
			int multiplier = 1; if (accidentalTriple) { multiplier = 3; } else if (accidentalDouble) { multiplier = 2; }

			if (hitIntended) { thisDartScoreReduction = goal * multiplier; }
			else { thisDartScoreReduction = (random::randChance(50) ? board.getLeftOf(goal) : board.getRightOf(goal)) * multiplier; }
		}
		// Shoot for double
		else if (goal <= 40 && (goal % 2 == 0)) {
			int baseNumber = goal / 2;

			bool hitIntendedNumber = random::randChance(playerInst.pointer->getOnTargetChance());
			bool hitDouble = random::randChance(playerInst.pointer->getDoubleChance());

			int multiplier = 1; if (hitDouble) { multiplier = 2; }

			if (hitIntendedNumber) { thisDartScoreReduction = baseNumber * multiplier; }
			else { thisDartScoreReduction = (random::randChance(50) ? board.getLeftOf(baseNumber) : board.getRightOf(baseNumber)) * multiplier; }
		}
		// Shoot for triple
		else if (goal <= 60 && (goal % 3 == 0)) {
			int baseNumber = goal / 3;

			bool hitIntendedNumber = random::randChance(playerInst.pointer->getOnTargetChance());
			bool hitTriple = random::randChance(playerInst.pointer->getTripleChance());

			int multiplier = 1; if (hitTriple) { multiplier = 3; }

			if (hitIntendedNumber) { thisDartScoreReduction = baseNumber * multiplier; }
			else { thisDartScoreReduction = (random::randChance(50) ? board.getLeftOf(baseNumber) : board.getRightOf(baseNumber)) * multiplier; }
		}

		scoreReduction += thisDartScoreReduction;

		if (output) {
			std::string numWords[3] = {"First", "Second", "Third"};
			print(numWords[i]," dart attempted to hit ", goal);
			if (goal == thisDartScoreReduction) { println(" successfully."); }
			else { println(", hit ", thisDartScoreReduction,"."); }
		}
	}

	bool canReduce = playerInst.score - scoreReduction > 1 || (playerInst.score - scoreReduction == 0 && finalWorkingScore % 2 == 0);
	if (canReduce) { playerInst.score -= scoreReduction; }

	if (output) {
		if (!canReduce) { println("Score was invalid, hits discarded."); }
		println("Score: ", playerInst.score);
		println();
	}

	playerInst.turn++;
	return playerInst.score == 0;
}