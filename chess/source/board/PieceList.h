#pragma once
#ifndef _Piece_List_h
#define _Piece_List_h
#include "Piece.h"
#include "source/SharedTypes.h"
#include "source/comon.h"
#include "source/board/board.h"
#include <map>
using namespace std;
using pieces_t = map<int, Piece>;
using const_iterator_t = pieces_t::const_iterator;

class PieceList {

public:
	PieceList(PlayerColor ourColor);
	void movePiece(Square from, Square to);
	void removePiece(Square square);
	void clearList();
	void add(Piece piece, Square sq);
	const_iterator_t begin() const;
	const_iterator_t end() const;
	int getPieceType(Square square);
	void updateMovement(Move move, const Board* board, PlayerColor movingSide);
	bitBoard_t getAttackingBB() const;
	bitBoard_t getOccupiesBB() const;
	 Piece* getPieceOnSquare(Square square) const;

private:
	pieces_t pieces; 
	bitBoard_t attackingBB;
	bitBoard_t occupiesBB;
	PlayerColor ownColor;

};

#endif
