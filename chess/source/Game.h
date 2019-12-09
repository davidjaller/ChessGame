///////////////////////////////////////////////////////////////////////////////
// Class Game
// Controls the happenings of the chessgame
///////////////////////////////////////////////////////////////////////////////

#ifndef _GAME_
#define _GAME_
#pragma once

// ------ Includes -----
#include <fstream>
#include <iostream>
#include <time.h>
#include <vector>

#include "source/board/Board.h"
#include <source/RulesManager.h>
#include "source/SharedTypes.h"
#include "source/io/humanPlayer.h"
#include "source/io/moveVisualiserPlayer.h"
#include "source/engine/computerPlayer.h"
#include "source/Player.h"
#include "source/io/Scene.h"


//class HumanPlayer;

class Game
{
public:

	Game (Scene *pScene, Board *pBoard);
	~Game();
	bool StepGame();
	void NewGame();

private:
	Scene *myScene;
	Board *gameBoard;
	int player;
	vector<unique_ptr<Player>> players;
};

#endif // _GAME_
