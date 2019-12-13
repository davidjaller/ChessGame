#ifndef _player_h
#define _player_h
#include "SharedTypes.h"
#include "source/board/position.h"
#include "source/RulesManager.h"
#include "source/engine/evaluator.h"
#include "source/engine/moveGenerator.h"
class Player {
	public:
		Player(Position *pPosition, PlayerColor color);
		~Player();
		virtual bool stepTurn() = 0; // abstract method
		virtual void init() = 0;

		void SetColor(PlayerColor color);
		PlayerColor GetColor();

		bool isChecked();
		bool canMove();
		void generateMoves();
		void resetMoves();
	private:
		vector<Square> kingAttackers;
	protected:
		Position* gamePosition;
		PlayerColor color;
		list<Move> possibleMoves;

};




#endif