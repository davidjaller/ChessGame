#include "source/io/moveVisualiserPlayer.h"

MoveVisualizerPlayer::MoveVisualizerPlayer(PlayerColor color, Scene* pScene, Position* pPosition) : Player(pPosition, color)
 {
	SetColor(color);

	myScene = pScene;

	init();
	state = 0;
}

void MoveVisualizerPlayer::init() {
	tempPosition = *gamePosition;
	turnDone = false;
}

///////////////////////////////////////////////////////////////////////////////
// StepTurn
// Do our possible moves one by one, then reset
///////////////////////////////////////////////////////////////////////////////
bool MoveVisualizerPlayer::stepTurn()
{	
	cout << " Score:   " << Evaluator::evaluatePosition(gamePosition, color) << endl;


	for (list<Move>::iterator it = possibleMoves.begin(); it != possibleMoves.end(); ++it) {
		tempPosition = *gamePosition;
		gamePosition->makeMoveFromTo(it->from, it->to);
		myScene->UpdateScreen();
		myScene->CreateScene();
		Sleep(200);
		*gamePosition = tempPosition;
		myScene->UpdateScreen();
		myScene->CreateScene();
		Sleep(200);

	}


	return true;
}
