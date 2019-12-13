#include "source/engine/computerPlayer.h"

ComputerPlayer::ComputerPlayer(PlayerColor color, Scene* pScene,  Position* pPosition) : Player(pPosition, color) {
	nodeCount = 0;
	scene = pScene;
}

void ComputerPlayer::init(){
}

bool ComputerPlayer::stepTurn() {

	// actually, Game should never allow us to play if allready mate or draw so should allways be true

	clock_t begin = clock();

	//code_to_time();
	bool success = findBestMove();
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Nodes reached: " << nodeCount << " Time elapsed: " << elapsed_secs << " seconds" << endl;
	

	if (success) {
		gamePosition->makeMoveFromTo(bestMove.from, bestMove.to);
		scene->CreateScene();

		scene->MarkSquare(bestMove.from);
		scene->MarkSquare(bestMove.to);
		scene->UpdateScreen();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		scene->CreateScene();
	}
	else {
		cout << "ComputerPlayer : Could not find a move" << endl;
	
	}
	return true;
}

float ComputerPlayer::minMaxRecursive(Position position, int level) {
	if (level >= MAX_SEARCH_DEPTH) {
		// add a small random while evaluation is simple and search shallow, 
		// so that we dont get same everytime
		float random = (float)(rand() % 100) / 1000;
		return Evaluator::evaluatePosition(&position, color)+random;
	}

	bool maximizing = position.getTurn() == color;

	Square sq;
	list<Move> moveList;
	int nrMoves = MoveGenerator::generateAll(&position, &moveList);
	if (nrMoves == 0) { // can't not move
		vector<Square> sqL;
		if (RulesManager::KingIsChecked(&position, &sqL, position.getTurn())) // mate
			if (maximizing)
				return std::numeric_limits<float>::min();
			else
				return std::numeric_limits<float>::max();
		else { // draw
			if (maximizing)
				return std::numeric_limits<float>::min()/2;
			else
				return std::numeric_limits<float>::max()/2;
		}
	}
	float maxScore = -std::numeric_limits<float>::max();
	float minScore = std::numeric_limits<float>::max();
	for (list<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
		Position newBoard = position;
		Move move = *it;
		
		newBoard.makeMoveFromTo(move.from, move.to);
		// is in check? (or generator will take care of that?)
		newBoard.setTurn(getOpposite(position.getTurn()));
		float score = minMaxRecursive(newBoard, level + 1);
		if (level == 0) {
			printMoveScore(move, score);
		}

		if (maximizing) {
			if (score >= maxScore) {
				maxScore = score;
				if (level == 0)
					bestMove = move;
			}
		}
		else {
			minScore = min(minScore, score);
		}
		nodeCount++;
		
	}
	if (maximizing)
		return maxScore;
	else
		return minScore;

}

bool ComputerPlayer::findBestMove() {
	int level = 0;
	nodeCount = 0;
	bool success = false;

	// consider starting this as a thread
	float score = minMaxRecursive(*gamePosition, level);
	if (nodeCount > 0) { //root node not counted
		success = true;
	}
	return success;
}
