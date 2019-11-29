#ifndef _player_h
#define _player_h
#include "SharedTypes.h"
class Player {
	public:
		Player();
		~Player();
		virtual bool stepTurn() = 0; // abstract method
		void SetColor(PlayerColor color);
		PlayerColor GetColor();
	private:
		PlayerColor color;
};




#endif