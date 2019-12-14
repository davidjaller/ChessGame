#include "source/io/humanPlayer.h"

//HumanPlayer::HumanPlayer() {};

HumanPlayer::HumanPlayer(PlayerColor color, Scene* pScene, Position* pPosition) : Player(pPosition, color) 
 {
	SetColor(color);

	squareFrom.file = 0;
	squareFrom.rank = 0;
	squareTo.file = 0;
	squareTo.rank = 0;
	scene = pScene;

	init();
	state = HumanPlayerState::waitFirstInput;
}

void HumanPlayer::init() {
	tempPosition = *gamePosition;
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

				if (gamePosition->IsFriendlyPiece(squareFrom))
				{
					scene->MarkSquare(squareFrom); // Mark square with active piece
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

				if (!gamePosition->IsFriendlyPiece(squareTo))
				{
					scene->MarkSquare(squareTo);
					state = HumanPlayerState::executeMove;

				}
				else
				{
					scene->CreateScene();
					state = HumanPlayerState::waitFirstInput; // If friendly piece choosed again, dont mark
				}
			}
		}
	}
	//------------------------------------------------
	else if (state == HumanPlayerState::executeMove)
	{
		if (gamePosition->IsLegalMove(squareFrom, squareTo))
		{
			vector<Square> attackingSquares;
			// Make move on temporary  board and se if king becomes or remaines threatened
			tempPosition = *gamePosition;
			tempPosition.makeMoveFromTo(squareFrom, squareTo);
			list<const Piece*> attackingPieces;
			if(!tempPosition.KingIsChecked(&attackingPieces, color))
			{
				*gamePosition = tempPosition;

				scene->CreateScene();
				scene->MarkSquare(squareFrom);
				scene->MarkSquare(squareTo);
				scene->UpdateScreen();
				Sleep(500);
				scene->CreateScene();

				// check for check mate should happen here
				turnDone = true;
				state = HumanPlayerState::waitFirstInput;

			}
			else
			{
				// If king is checked, take back temporary move and mark the squares
				// from which the attack comes
				tempPosition = *gamePosition;
				for (vector<Square>::iterator it = attackingSquares.begin(); it != attackingSquares.end(); ++it) {
					scene->MarkSquare(*it);
				}
				scene->UpdateScreen();
				Sleep(200);
				scene->CreateScene();
				state = HumanPlayerState::waitFirstInput;

			}
		}
		else
		{
			// Move not legal
			scene->CreateScene();
			state = HumanPlayerState::waitFirstInput;
		}
	}
	return turnDone;
}

///////////////////////////////////////////////////////////////////////////////
// positionWithinBoard
//
///////////////////////////////////////////////////////////////////////////////
bool HumanPlayer::positionWhitinBoard(int file, int rank)
{
	if (file > scene->x1&& file < scene->x2 && rank > scene->y1&& rank < scene->y2)
		return true;
	else
		return false;
}

///////////////////////////////////////////////////////////////////////////////
// coordinatesToSquare
//
///////////////////////////////////////////////////////////////////////////////
void HumanPlayer::coordinatesToSquare(int file, int rank, Square* square) {
	square->file = int(floor(float(file - scene->x1) / float(SQUARE_SIZE)));
	square->rank = int(floor(float(rank - scene->y1) / float(SQUARE_SIZE)));
}