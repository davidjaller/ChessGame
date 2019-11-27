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

#include "source/Board.h"
#include "source/Scene.h"
#include <source/RulesManager.h>
#include "Source/SharedConstants.h"

static Player player; // TODO: prefearably this would be a private member of Game

class Game
{
public:

	Game (Scene *pScene, Board *pBoard);
	void GameStep();
	void NewGame();
	static Player getPlayer();

private:
	Scene *myScene;
	Board *myBoard;
	GameState gameState;
	

	Square squareFrom;
	Square squareTo;
	
	Board tempBoard;

	bool positionWhitinBoard(int x, int y);
	void coordinatesToSquare(int x, int y, Square* square);
};

#endif // _GAME_
