#ifndef _human_player_h
#define _human_player_h
#include "source/Player.h"
#include "source/board/Board.h"
#include "source/SharedTypes.h"
#include "source/board/Board.h"
#include <source/RulesManager.h>
#include "source/io/Scene.h"
#include <time.h>
#include <windows.h>

class HumanPlayer : public Player {
public:
	HumanPlayer();
	HumanPlayer(PlayerColor color, Board *pBoard, Scene *pScene);
	void init();
	bool stepTurn();
	bool canMove();
	bool isChecked();
private:

	Board* gameBoard; // pointer to global board
	Board tempBoard = Board(PlayerColor::WHITE); // humanPlayers own  
	Scene* myScene;
	HumanPlayerState state;

	Square squareFrom;
	Square squareTo;

	bool positionWhitinBoard(int x, int y);
	void coordinatesToSquare(int x, int y, Square* square);
	bool turnDone;
};
#endif