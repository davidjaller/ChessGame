#pragma once
#ifndef _piece_h
#define _piece_h
#include "source/SharedTypes.h"
#include "source/board/board.h"
#include "source/comon.h"

class Piece {
public:
	Piece(int pieceType, Square ownSquare, PlayerColor ownColor);
	int getType();
	bitBoard_t canMoveToBB;
	bitBoard_t blockedOnBB;
	bitBoard_t ownPositionBB;
	bitBoard_t canKillOnBB; //perhabs not needed since easy to calculate with canMoveTo&oppOccupied(except for pawns)
	bitBoard_t pawnAttacksBB;
	Square ownSquare;
	// getters for bitboards
	void updateMovement(const Board* board);
	bool isSlider();

private:
	int pieceType;
	PlayerColor ownColor;

	void generateForPawn(const Board* board);
	void generateForBishop(const Board* board);
	void generateForKnight(const Board* board);
	void generateForRook(const Board* board);
	void generateForQueen(const Board* board);
	void generateForKing(const Board* board);
	void generateForSliders(const Board* board, vector<array<int, 2>> directions);
	bool isOurEnemy(Square sq, const Board* board);
	bool isOurFriend(Square sq, const Board* board);

};

#endif
