///////////////////////////////////////////////////////////////////////////////
// Class Game
// Controls the happenings of the chessgame
///////////////////////////////////////////////////////////////////////////////

#ifndef _GAME_
#define _GAME_

// ------ Includes -----
#include <fstream>
#include <iostream>
#include <time.h>

#include "source/board/Board.h"
#include <source/RulesManager.h>
#include "source/SharedTypes.h"
#include "source/io/humanPlayer.h"
#include "source/Player.h"
#include "source/io/Scene.h"

static PlayerColor player; // TODO: prefearably this would be a private member of Game

class Game
{
public:

	Game (Scene *pScene, Board *pBoard);
	~Game();
	void StepGame();
	void NewGame();

private:
	Scene *myScene;
	Board *gameBoard;
	int player;
	HumanPlayer player1;
	HumanPlayer player2;
};

#endif // _GAME_
