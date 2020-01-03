#include "source/engine/computerPlayer.h"

ComputerPlayer::ComputerPlayer(PlayerColor color, Scene* pScene,  Position* pPosition) : Player(pPosition, color) {
	nrEvaluatedPositions = 0;
	scene = pScene;
}

void ComputerPlayer::init(){
}

bool ComputerPlayer::stepTurn() 
{
	clock_t begin = clock();

	//code_to_time();
	bool success = findBestMove();
	//if (!MoveGenerator::IsLegalMove(gamePosition, bestMove.from, bestMove.to))
		//throw("Error, confliction between IsLegal and generateMoves");

//#ifdef DEBUG_MODE
	//const Piece* piece = gamePosition->getPieceOnSquare(bestMove.from, color);
	//const Piece* piece = gamePosition->getPieceOnSquare(Square(7,0), color);
	//cout << "best move is:" << endl;
	//printMove(bestMove);
	//printBitBoard("move:"+to_string(piece->type), piece->canMoveToBB);
	//printBitBoard("kill", piece->canKillOnBB);
//#endif
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Nodes reached: " << nrEvaluatedPositions << " Time elapsed: " << elapsed_secs << " seconds" << endl;
	
	if (success) {
		gamePosition->makeMoveFromTo(bestMove.from, bestMove.to);
		printFen(gamePosition->getBoard());
		scene->CreateScene();

		scene->MarkSquare(bestMove.from);
		scene->MarkSquare(bestMove.to);
		scene->UpdateScreen();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		//scene->CreateScene();
	}
	else 
	{
		cout << "ComputerPlayer : Could not find a move" << endl;
	}
	return true;
}

float ComputerPlayer::minMaxRecursive(Position position, Move move, int level)
{
	if (level > 0) 
	{
		position.makeMoveFromTo(move.from, move.to);
		position.setTurn(getOpposite(position.getTurn()));
	}
	if (level >= MAX_SEARCH_DEPTH) 
	{
		// add a small random while evaluation is simple and search shallow, 
		// so that we dont get same everytime
		float random = (float)(rand() % 100) / 1000;
		nrEvaluatedPositions++;
		return Evaluator::evaluatePosition(&position, color)+random;
	}
	bool maximizing = position.getTurn() == color;

	Square sq;
	list<Move> moveList;
	int nrMoves = MoveGenerator::generateMoves(&position, &moveList, true); //captures first
	nrMoves += MoveGenerator::generateMoves(&position, &moveList, false); //pushes after (good for pruning)
	if (nrMoves == 0) // can't move
	{ 
		list<const Piece*> pieces;
		if (position.KingIsChecked(&pieces, position.getTurn())) // mate
			if (maximizing)
				return std::numeric_limits<float>::min();
			else
				return std::numeric_limits<float>::max();
		else // draw
			return 0; // this way we are happy with draw compared to a slight disadvantage, consider a threashold
	}
	float maxScore = -std::numeric_limits<float>::max();
	float minScore = std::numeric_limits<float>::max();
	for (list<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) 
	{
		Move move = *it;
		float score = minMaxRecursive(position, move, level + 1);
		if (level == 0) {
			printMoveScore(move, score);
		}
		if (maximizing) 
		{
			if (score >= maxScore) 
			{
				maxScore = score;
				if (level == 0)
					bestMove = move;
			}
		}
		else 
			minScore = min(minScore, score);	
	}
	if (maximizing)
		return maxScore;
	else
		return minScore;
}

bool ComputerPlayer::findBestMove() 
{
	int level = 0;
	nrEvaluatedPositions = 0;
	bool success = false;
	Move move;

	// consider starting this as a thread
	float score = minMaxRecursive(*gamePosition, move, level);
	if (nrEvaluatedPositions > 0) 
	{ 
		success = true;
	}
	return success;
}
