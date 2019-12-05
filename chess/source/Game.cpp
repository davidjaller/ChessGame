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
#include "source/io/moveVisualiserPlayer.h"

extern array<array<int, 8>, 8> INIT_BOARD;

///////////////////////////////////////////////////////////////////////////////
// Constructor
//
///////////////////////////////////////////////////////////////////////////////

Game::Game(Scene *pScene, Board *pBoard)// : player1(PlayerColor::WHITE, pScene, pBoard), player2(PlayerColor::BLACK, pScene, pBoard)
{
	gameBoard = pBoard;
	myScene = pScene;
	player = 0;
	players.push_back(unique_ptr<Player>(new MoveVisualizerPlayer(PlayerColor::WHITE, pScene, pBoard)));
	//players.push_back(unique_ptr<Player> (new HumanPlayer(PlayerColor::WHITE, pScene, pBoard)));
	players.push_back(unique_ptr<Player> (new HumanPlayer(PlayerColor::BLACK, pScene, pBoard)));
	
}

Game::~Game() {}

///////////////////////////////////////////////////////////////////////////////
// NewGame

///////////////////////////////////////////////////////////////////////////////
void Game::NewGame()
{
	players[0]->init();
	players[1]->init();
	gameBoard->InitBoard(INIT_BOARD, PlayerColor::WHITE);
	myScene->CreateScene();
	player = 0;
}

void Game::StepGame() {
		
		// Add Check for check mate or draw here!

		bool canMove = players[player]->generateMoves();
		if (players[player]->stepTurn()) { //returns done if finished
			player = !player;
			gameBoard->setTurn(players[player]->GetColor());
		}

}




