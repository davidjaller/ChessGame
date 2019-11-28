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
	myBoard = pBoard;
	myScene = pScene;
	gameState = GameState::waitFirstInput;
	player = Player::WHITE;

	squareFrom.x = 0;
	squareFrom.y = 0;
	squareTo.x = 0;
	squareTo.y = 0;
}

Game::~Game() {}

///////////////////////////////////////////////////////////////////////////////
// NewGame

///////////////////////////////////////////////////////////////////////////////
void Game::NewGame()
{
	tempBoard.InitBoard(INIT_BOARD);
	myBoard->InitBoard(INIT_BOARD);
	myScene->CreateScene();
}

///////////////////////////////////////////////////////////////////////////////
// GameStep
// State machine that waits for user input, checks if legal, and if so makes move
///////////////////////////////////////////////////////////////////////////////
void Game::GameStep()
{

	if(gameState == GameState::waitFirstInput)
	{
		//wait for first choice
		SDL_Event event;
		SDL_WaitEvent(&event);
		
		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (positionWhitinBoard(event.button.x, event.button.y) == true)
			{
				coordinatesToSquare(event.button.x, event.button.y, &squareFrom);

				if (myBoard->IsFriendlyPiece(squareFrom))
				{
					myScene->MarkSquare(squareFrom); // Mark square with active piece
					gameState = GameState::waitSecondInput;
				}
			}

		}
	}
	//--------------------------------------------------------------------
	else if(gameState == GameState::waitSecondInput)
	{
		// wait for second choice
		SDL_Event event;
		SDL_WaitEvent(&event);
		
			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				if(positionWhitinBoard(event.button.x, event.button.y) == true)
				{
					coordinatesToSquare(event.button.x, event.button.y, &squareTo);
					
					if(!myBoard->IsFriendlyPiece(squareTo))
					{
						myScene->MarkSquare(squareTo); 
						gameState = GameState::executeMove;
						
					}
					else
					{
						myScene->CreateScene();
						gameState = GameState::waitFirstInput; // If friendly piece choosed again, dont mark
					}
				}
			}	
	}
	//------------------------------------------------
	else if (gameState == GameState::executeMove)
	{
		if(RulesManager::IsLegalMove(myBoard, squareFrom,squareTo))
		{
			Square attackingSquare;
			// Make move on temporary  board and se if king becomes or remaines threatened
			tempBoard.makeMoveFromTo(squareFrom, squareTo);
			 if(!RulesManager::KingIsChecked(&tempBoard, &attackingSquare))
				{
				*myBoard = tempBoard;
				
				myScene->CreateScene();
				myScene->MarkSquare(squareFrom);
				myScene->MarkSquare(squareTo);
				myScene->UpdateScreen();
				Sleep(500);
				myScene->CreateScene();

				// check for check mate should happen here
				player = (Player)!(int)player; // turn is over, toggle player
				gameState = GameState::waitFirstInput;
				
			}
			else
			{
				// If king is checked, take back temporary move and mark the square
				// from which the attack comes
				tempBoard = *myBoard;
				myScene->MarkSquare(attackingSquare);
				myScene->UpdateScreen();
				Sleep(200);
				myScene->CreateScene();
				gameState = GameState::waitFirstInput;

			}
		}
		else
		{
			// Move not legal
			myScene->CreateScene();
			gameState = GameState::waitFirstInput;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// positionWithinBoard
//
///////////////////////////////////////////////////////////////////////////////
bool Game:: positionWhitinBoard(int x, int y)
{
	 if(x > myScene->x1 && x < myScene->x2 && y > myScene->y1 && y < myScene-> y2)
		 return true;
	 else
		 return false;
}

///////////////////////////////////////////////////////////////////////////////
// coordinatesToSquare
//
///////////////////////////////////////////////////////////////////////////////
void Game::coordinatesToSquare(int x, int y, Square *square) {
	square->x = int(floor(float(x - myScene->x1) / float(SQUARE_SIZE)));
	square->y = int(floor(float(y - myScene->y1) / float(SQUARE_SIZE)));
}

Player Game::getPlayer() {
	return player;
}



