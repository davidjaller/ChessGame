
#ifndef _human_player_h
#define _human_player_h
#pragma once

#include "source/Player.h"
#include "source/SharedTypes.h"
#include "source/io/Scene.h"
#include <time.h>
#include <windows.h>

enum class HumanPlayerState { waitFirstInput, waitSecondInput, executeMove };

class HumanPlayer : public Player {
public:
	HumanPlayer(PlayerColor color, Scene* pScene, Position* pPosition);
	void init();
	bool stepTurn();
	//bool canMove();
	//bool isChecked();
private:

	Position tempPosition;// = Position(PlayerColor::WHITE); // humanPlayers own  
	Scene* scene;
	HumanPlayerState state;

	Square squareFrom;
	Square squareTo;

	bool positionWhitinBoard(int file, int rank);
	void coordinatesToSquare(int file, int rank, Square* square);
	bool turnDone;
};
#endif