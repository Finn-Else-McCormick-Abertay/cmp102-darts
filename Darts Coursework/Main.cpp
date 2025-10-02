#include "Player.h"
#include "Game.h"
#include "Championship.h"

#include "IOUtil.h"
#include "RandUtil.h"

#include <vector>

int main() {
	int gameType;
	do { print("Enter game (301/501): "); gameType = input::getInt(); } while (gameType != 301 && gameType != 501);
	println();

	int mode;
	println("Modes: \n - (S)ingle game\n - (M)onte Carlo\n - (C)hampionship");
	do { print("Enter mode: "); mode = toupper(input::getChar()); }
	while (mode != 'S' && mode != 'M' && mode != 'C'); // Keep asking until get valid input
	println();

	int numGames = 1;
	if (mode != 'S') {
		print("Enter number of games to run: "); numGames = input::getInt(); if (numGames <= 0) { numGames = 10000; println("Defaulting to 10,000."); }
		println();
	}

	std::vector<Player> players;

	print("Use default players? (Y/N): ");
	bool useDefault = (toupper(input::getChar()) == 'Y');
	println();

	if (useDefault) { players = { Player("Joe",71,80,75,69), Player("Sid",73,80,76,69) }; }
	else {
		bool keepAddingPlayers = true;
		while (keepAddingPlayers) {
			println(" - Add Player -");
			std::cin.ignore();
			std::string name; do { print("Enter name: "); name = input::getString(); } while (name == "");
			println("Enter probabilities (as percentages, but without the % symbol):");
			double bull; do { print("Bullseye chance: "); bull = input::getDouble(); } while (bull < 0 || bull > 100);
			double onTarget; do { print("On target chance: "); onTarget = input::getDouble(); } while (onTarget < 0 || onTarget > 100);

			double doubleChance = bull; double tripleChance = bull;
			if (gameType == 501) {
				do { print("Double chance: "); doubleChance = input::getDouble(); } while (doubleChance < 0 || doubleChance > 100);
				do { print("Triple chance: "); tripleChance = input::getDouble(); } while (tripleChance < 0 || tripleChance > 100);
			}
			players.push_back(Player(name, bull, onTarget, doubleChance, tripleChance));

			print("Add another? (Y/N): ");
			keepAddingPlayers = (toupper(input::getChar()) == 'Y');
			println();
		}
	}

	if (mode != 'C') { // order in championship is procedurally determined, so this step is pointless there
		print("Change player order? (Y/N): ");
		bool changeOrder = (toupper(input::getChar()) == 'Y');
		println();
		while (changeOrder) {
			println("Current order:");
			for (int i = 0; i < players.size(); i++) { println((i + 1), " | ", players[i].getName()); }
			int pos; do { print("Enter position of player to change: "); pos = input::getInt()-1; } while (pos < 0 || pos >= players.size());
			int newPos; do { print("Enter new position: "); newPos = input::getInt() - 1; } while (newPos < 0 || newPos >= players.size());
			Player movingPlayer = players[pos];
			players.erase(players.begin()+pos);
			players.insert(players.begin()+newPos,movingPlayer);

			println("\nModified order:");
			for (int i = 0; i < players.size(); i++) { println((i + 1), " | ", players[i].getName()); }
			println();

			print("Keep changing? (Y/N): ");
			changeOrder = (toupper(input::getChar()) == 'Y');
			println();
		}
	}


	random::init();
	Game* game = NULL;
	if (gameType == 301) { game = new Darts301(); }
	else if (gameType == 501) { game = new Darts501(); }

	/* Have to use a vector of pointers instead of the actual vector of players because the championship needs
	   to call the game's play function from a scope where it can't access a reference to the player vector */
	std::vector<Player*> playerPointers; for (Player& player : players) { playerPointers.push_back(&player); }

	bool playAgain = false;
	do {
		if (mode == 'C') {
			Championship championship(game, playerPointers);
			championship.runRepeated(numGames);
		}
		else {
			for (int i = 0; i < numGames; i++) { game->play(playerPointers, mode == 'S'); }

			if (mode != 'S') {
				set_output_precision(3);
				println();
				println(" - Stats - \n");
				for (Player& player : players) {
					println(player.getName(), ":");
					println("Wins: ", player.getNumWins());
					println("Average Win Chance: ", player.getWinAverage(), "%");
					println("Average Turns to Win: ", player.getAverageTurnsToWin());
					println();
				}
			}
		}
		println();
		print("Play again? (Y/N): ");
		playAgain = (toupper(input::getChar()) == 'Y');
		println();
	} while (playAgain);

	delete game;
}