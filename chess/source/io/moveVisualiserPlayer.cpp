#include "source/io/moveVisualiserPlayer.h"

MoveVisualizerPlayer::MoveVisualizerPlayer(PlayerColor color, Scene* pScene, Board* pBoard) : Player(pBoard, color)
 {
	SetColor(color);

	myScene = pScene;

	init();
	state = 0;
}

void MoveVisualizerPlayer::init() {
	tempBoard = *gameBoard;
	turnDone = false;
}

///////////////////////////////////////////////////////////////////////////////
// StepTurn
// Do our possible moves one by one, then reset
///////////////////////////////////////////////////////////////////////////////
bool MoveVisualizerPlayer::stepTurn()
{	
	cout << " Score:   " << Evaluator::evaluatePosition(gameBoard, color) << endl;


	for (list<Move>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it) {
		tempBoard = *gameBoard;
		gameBoard->makeMoveFromTo(it->from, it->to);
		myScene->UpdateScreen();
		myScene->CreateScene();
		Sleep(200);
		*gameBoard = tempBoard;
		myScene->UpdateScreen();
		myScene->CreateScene();
		Sleep(200);

	}


	return true;
}
