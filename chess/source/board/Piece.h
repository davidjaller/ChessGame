#pragma once
#ifndef _piece_h
#define _piece_h
#include "source/SharedTypes.h"

class Piece {
public:
	Piece(int pieceType);
	int getType();
	// getters for bitboards
	// updateBitboards()
	// isBlocking(attackFromSquare, attackToSquare)
	// isAttacking(square)
private:
	int pieceType;
	// bitboards for mobility, attacks etc. 


};

#endif
