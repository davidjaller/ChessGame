#ifndef _player_h
#define _player_h
#include "SharedTypes.h"
#include "source/board/Board.h"
class Player {
	public:
		Player(Board *pBoard, PlayerColor color);
		~Player();
		virtual bool stepTurn() = 0; // abstract method
		virtual void init() = 0;

		void SetColor(PlayerColor color);
		PlayerColor GetColor();

		//public isChecked();
		//public canMove();
	private:
		PlayerColor color;
	protected:
		Board* gameBoard;

};




#endif