///////////////////////////////////////////////////////////////////////////////
// Class Game
// Controls the happenings of the chessgame
///////////////////////////////////////////////////////////////////////////////

// ------ Includes -----

#ifndef __linux__
#include <windows.h>
#endif
#include <math.h>

#include "Game.h"
#include "source/io/moveVisualiserPlayer.h"

extern array<array<int, 8>, 8> INIT_BOARD;

///////////////////////////////////////////////////////////////////////////////
// Constructor
//
///////////////////////////////////////////////////////////////////////////////

Game::Game(Scene *pScene, Position *pPosition)// : player1(PlayerColor::WHITE, pScene, pPosition), player2(PlayerColor::BLACK, pScene, pPosition)
{
	gamePosition = pPosition;
	myScene = pScene;
	player = 0;
	//players.push_back(unique_ptr<Player>(new MoveVisualizerPlayer(PlayerColor::WHITE, pScene, pPosition)));
	//players.push_back(unique_ptr<Player> (new HumanPlayer(PlayerColor::WHITE, pScene, pPosition)));
	players.push_back(unique_ptr<Player> (new ComputerPlayer(PlayerColor::WHITE, pScene, pPosition)));
	players.push_back(unique_ptr<Player> (new HumanPlayer(PlayerColor::BLACK, pScene, pPosition)));
	
}

Game::~Game() {}

///////////////////////////////////////////////////////////////////////////////
// NewGame

///////////////////////////////////////////////////////////////////////////////
void Game::NewGame()
{
	players[0]->init();
	players[1]->init();
	gamePosition->InitPosition(INIT_BOARD, PlayerColor::WHITE);
	//gamePosition->InitPosition("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	//gamePosition->InitPosition("rnq4r/ppp3bp/8/4P3/6k1/8/PPPK1P1P/R6R/");

	gamePosition->updateAllPieces();
	myScene->CreateScene();
	myScene->UpdateScreen();
	player = 0;
	previousPosition = *gamePosition;
}

void Game::resetToPrevious() {
	player = !player;
	*gamePosition = previousPosition;
	myScene->CreateScene();
	myScene->UpdateScreen();
}



bool Game::StepGame() 
{

	bool finnished = false;
	Position tempPosition = *gamePosition;
	if (players[player]->stepTurn()) { //returns done if finished
		previousPosition = tempPosition;
		
		player = !player;
		gamePosition->setTurn(players[player]->GetColor());
		//if (!players[player]->canMove()) {
		//	if (players[player]->isChecked()) {
		//		cout << "player " << !player << "is winner" << endl;
		//	}
		//	else
		//		cout << "game is draw" << endl;
			//finnished = true;
		//}
	}

	return finnished;
}




