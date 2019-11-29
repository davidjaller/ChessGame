///////////////////////////////////////////////////////////////////////////////
// Class Game
// Controls the happenings of the chessgame
///////////////////////////////////////////////////////////////////////////////

// ------ Includes -----
#ifndef LINUX
#include <windows.h>
#include <math.h>
#endif
#include "Game.h"

extern int INIT_BOARD[8][8];

///////////////////////////////////////////////////////////////////////////////
// Constructor
//
///////////////////////////////////////////////////////////////////////////////

Game::Game(Scene *pScene, Board *pBoard)
{
	gameBoard = pBoard;
	myScene = pScene;
	player1 = HumanPlayer(PlayerColor::WHITE, pBoard, pScene);
	player2 = HumanPlayer(PlayerColor::BLACK, pBoard, pScene);
	player = 1;
}

Game::~Game() {}

///////////////////////////////////////////////////////////////////////////////
// NewGame

///////////////////////////////////////////////////////////////////////////////
void Game::NewGame()
{
	player1.init();
	player2.init();
	gameBoard->InitBoard(INIT_BOARD, PlayerColor::WHITE);
	myScene->CreateScene();
	player = 1;
}

void Game::StepGame() {
	if (player == 1){
		// Add here:
		// generate moves
		// check if can move
		// if player is computer, use the moves generated for first level search
		// same for player 2

		// abstract player class could have methods for isChecked and Can move and the member board

		if (player1.stepTurn()) { //returns done when finished
			player = 2;
			gameBoard->setTurn(player2.GetColor());
		}
	}
	else if (player == 2) {
		if(player2.stepTurn()){
			player = 1;
			gameBoard->setTurn(player1.GetColor());
		}
	}

}





