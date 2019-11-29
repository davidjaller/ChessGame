#include "source/io/humanPlayer.h"

HumanPlayer::HumanPlayer() {};

HumanPlayer::HumanPlayer(PlayerColor color, Board *pBoard, Scene* pScene) {
	SetColor(color);

	squareFrom.x = 0;
	squareFrom.y = 0;
	squareTo.x = 0;
	squareTo.y = 0;
	gameBoard = pBoard;
	myScene = pScene;

	init();
	state = HumanPlayerState::waitFirstInput;
}

void HumanPlayer::init() {
	tempBoard = *gameBoard;
	turnDone = false;
}

///////////////////////////////////////////////////////////////////////////////
// StepTurn
// State machine that waits for user input, checks if legal, and if so makes move
///////////////////////////////////////////////////////////////////////////////
bool HumanPlayer::stepTurn()
{

	if (state == HumanPlayerState::waitFirstInput)
	{
		turnDone = false;
		//wait for first choice
		SDL_Event event;
		SDL_WaitEvent(&event);

		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (positionWhitinBoard(event.button.x, event.button.y) == true)
			{
				coordinatesToSquare(event.button.x, event.button.y, &squareFrom);

				if (gameBoard->IsFriendlyPiece(squareFrom))
				{
					myScene->MarkSquare(squareFrom); // Mark square with active piece
					state = HumanPlayerState::waitSecondInput;
				}
			}

		}
	}
	//--------------------------------------------------------------------
	else if (state == HumanPlayerState::waitSecondInput)
	{
		// wait for second choice
		SDL_Event event;
		SDL_WaitEvent(&event);

		if (event.type == SDL_MOUSEBUTTONDOWN)
		{
			if (positionWhitinBoard(event.button.x, event.button.y) == true)
			{
				coordinatesToSquare(event.button.x, event.button.y, &squareTo);

				if (!gameBoard->IsFriendlyPiece(squareTo))
				{
					myScene->MarkSquare(squareTo);
					state = HumanPlayerState::executeMove;

				}
				else
				{
					myScene->CreateScene();
					state = HumanPlayerState::waitFirstInput; // If friendly piece choosed again, dont mark
				}
			}
		}
	}
	//------------------------------------------------
	else if (state == HumanPlayerState::executeMove)
	{
		if (RulesManager::IsLegalMove(gameBoard, squareFrom, squareTo))
		{
			Square attackingSquare;
			// Make move on temporary  board and se if king becomes or remaines threatened
			tempBoard.makeMoveFromTo(squareFrom, squareTo);
			if (!RulesManager::KingIsChecked(&tempBoard, &attackingSquare))
			{
				*gameBoard = tempBoard;

				myScene->CreateScene();
				myScene->MarkSquare(squareFrom);
				myScene->MarkSquare(squareTo);
				myScene->UpdateScreen();
				Sleep(500);
				myScene->CreateScene();

				// check for check mate should happen here
				turnDone = true;
				state = HumanPlayerState::waitFirstInput;

			}
			else
			{
				// If king is checked, take back temporary move and mark the square
				// from which the attack comes
				tempBoard = *gameBoard;
				myScene->MarkSquare(attackingSquare);
				myScene->UpdateScreen();
				Sleep(200);
				myScene->CreateScene();
				state = HumanPlayerState::waitFirstInput;

			}
		}
		else
		{
			// Move not legal
			myScene->CreateScene();
			state = HumanPlayerState::waitFirstInput;
		}
	}
	return turnDone;
}

///////////////////////////////////////////////////////////////////////////////
// positionWithinBoard
//
///////////////////////////////////////////////////////////////////////////////
bool HumanPlayer::positionWhitinBoard(int x, int y)
{
	if (x > myScene->x1&& x < myScene->x2 && y > myScene->y1&& y < myScene->y2)
		return true;
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
// coordinatesToSquare
//
///////////////////////////////////////////////////////////////////////////////
void HumanPlayer::coordinatesToSquare(int x, int y, Square* square) {
	square->x = int(floor(float(x - myScene->x1) / float(SQUARE_SIZE)));
	square->y = int(floor(float(y - myScene->y1) / float(SQUARE_SIZE)));
}