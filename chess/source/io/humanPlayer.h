
#ifndef _human_player_h
#define _human_player_h
#pragma once

#include "source/Player.h"
#include "source/SharedTypes.h"
#include <source/RulesManager.h>
#include "source/io/Scene.h"
#include <time.h>
#include <windows.h>

class HumanPlayer : public Player {
public:
	HumanPlayer(PlayerColor color, Scene* pScene, Board* pBoard);
	void init();
	bool stepTurn();
	//bool canMove();
	//bool isChecked();
private:

	Board tempBoard;// = Board(PlayerColor::WHITE); // humanPlayers own  
	Scene* myScene;
	HumanPlayerState state;

	Square squareFrom;
	Square squareTo;

	bool positionWhitinBoard(int file, int rank);
	void coordinatesToSquare(int file, int rank, Square* square);
	bool turnDone;
};
#endif