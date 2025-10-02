#include "Championship.h"

Championship::ChampionshipPlayerInstance::ChampionshipPlayerInstance(Player* player) { pointer = player; sets = 0; games = 0; }


Championship::Championship(Game* game, std::vector<Player*>& playerPointers) {
	this->game = game;
	for (Player* player : playerPointers) {
		playerInstances.push_back(ChampionshipPlayerInstance(player));
	}
}

void Championship::runChampionship(int numSets, int numGamesPerSet) {
	for (ChampionshipPlayerInstance& instance : playerInstances) { instance.sets = 0; instance.games = 0; }
	for (int set = 0; set < numSets; set++) {
		bool championshipOver = false;
		int startIndex = random::randInt(0, (int)playerInstances.size()-1);
		for (int i = 0; i < numGamesPerSet; i++) {
			bool setOver = false;
			std::vector<Player*> playerPointers; // Create vector of player pointers to pass into game in order of play
			for (int j = startIndex; j < playerInstances.size(); j++) { playerPointers.push_back(playerInstances[j].pointer); }
			for (int j = 0; j < startIndex; j++) { playerPointers.push_back(playerInstances[j].pointer); }
			Player& winner = game->play(playerPointers);
			startIndex++; if (startIndex >= playerInstances.size()) { startIndex -= (int)playerInstances.size(); } // Increment, wrapping around

			for (ChampionshipPlayerInstance& instance : playerInstances) {
				if (instance.pointer == &winner) { instance.games++; }
				if (instance.games >= std::ceil((numGamesPerSet % 2 ? numGamesPerSet : numGamesPerSet + 1) / (double)2)) { // First to gain majority wins set
					instance.sets++; setOver = true;
				}
			}
			if (setOver) { break; }
		}
		for (ChampionshipPlayerInstance& instance : playerInstances) {
			instance.games = 0;
			if (instance.sets >= std::ceil((numSets % 2 ? numSets : numSets + 1) / (double)2)) { championshipOver = true; } // First to gain majority of sets wins
		}
		if (championshipOver) { break; }
	}
}


void Championship::runRepeated(int repetitions) {
	int numPlayers = (int)playerInstances.size();
	std::map<std::vector<int>, int> frequencies;

	for (int i = 0; i < repetitions; i++) {
		runChampionship();
		std::vector<int> sampleVector;
		for (ChampionshipPlayerInstance& instance : playerInstances) { sampleVector.push_back(instance.sets); }
		if (frequencies.contains(sampleVector)) { frequencies[sampleVector]++; } else { frequencies[sampleVector] = 1; }
	}

	std::vector<int> sumFx(numPlayers); std::vector<std::map<int, int>> setFreqs(numPlayers);
	std::vector<std::pair<double, std::vector<int>>> sortedFrequencies;

	for (auto& freqPair : frequencies) {
		std::vector<int> sample = freqPair.first; int freq = freqPair.second;
		for (int i = 0; i < numPlayers; i++) {
			if (setFreqs[i].contains(sample[i])) { setFreqs[i][sample[i]]++; } else { setFreqs[i][sample[i]] = 1; }
			sumFx[i] += sample[i] * freq;
		}
		double percentageFreq = ((double)freq / (double)repetitions) * 100;
		sortedFrequencies.push_back(make_pair(percentageFreq, sample));
	}
	std::sort(sortedFrequencies.begin(), sortedFrequencies.end(), [](auto& a, auto& b) { return b.first > a.first; });

	std::vector<double> mean(numPlayers); std::vector<int> mode(numPlayers);
	for (int i = 0; i < numPlayers; i++) {
		mean[i] = (double)sumFx[i] / (double)repetitions;
		std::pair<int, int> tempModalFreq = { 0,0 };
		for (auto& pair : setFreqs[i]) { if (pair.second > tempModalFreq.second) { tempModalFreq = pair; } }
		mode[i] = tempModalFreq.first;
	}
	
	std::vector<int> nameLength(numPlayers);
	std::vector<int> buffer(numPlayers);
	for (int i = 0; i < numPlayers; i++) {
		nameLength[i] = (int)playerInstances[i].pointer->getName().size();
		buffer[i] = std::max(nameLength[i] + 1, 5);
	}
	set_output_precision(3);
	println();
	for (int i = 0; i < numPlayers; i++) { print_whitespace(buffer[i] - nameLength[i]); print(playerInstances[i].pointer->getName(), " | "); }
	println("Frequency");
	for (auto& freqPair : sortedFrequencies) {
		double percentageFreq = freqPair.first; std::vector<int> sample = freqPair.second;
		for (int i = 0; i < numPlayers; i++) { print_whitespace(buffer[i] - 1); print(sample[i], " | "); }
		println(percentageFreq, "%");
	}
	for (int i = 0; i < numPlayers; i++) { for (int j = 0; j < buffer[i]; j++) { print("-"); } } println("---------");
	set_output_precision(2);
	for (int i = 0; i < numPlayers; i++) { print_whitespace(buffer[i] - ((mean[i] == std::floor(mean[i])) ? 1 : 3)); print(mean[i], " | "); }
	println("Mean");
	for (int i = 0; i < numPlayers; i++) { print_whitespace(buffer[i] - 1); print(mode[i], " | "); }
	println("Mode");
}