#pragma once
#ifndef _Piece_List_h
#define _Piece_List_h
#include "Piece.h"
#include "source/SharedTypes.h"
#include "source/comon.h"
#include "source/board/board.h"
#include <map>
#include <list>
using namespace std;
using pieces_t = map<int, Piece>;
using iterator_t = pieces_t::iterator;
using const_iterator_t = pieces_t::const_iterator;

class PieceList {

public:
	PieceList(PlayerColor ourColor);
	void movePiece(int pieceType, Square from, Square to);
	void removePiece(Square square);
	void clearList();
	void add(int pieceType, Square sq);
	const_iterator_t begin() const;
	const_iterator_t end() const;
	int getPieceType(Square square);
	void updateMovement(Move move, const Board* board, PlayerColor movingSide);
	bitBoard_t getAttackingBB() const;
	bitBoard_t getOccupiesBB() const;
	bool getPieceOnSquare(Square square, const Piece* piece) const;
	bool getPiecesAttackingPiece(Square attackedPieceSquare, list<const Piece*>* attackingPieces) const;

private:
	pieces_t pieces; 
	bitBoard_t attacksBB;
	bitBoard_t occupiesBB;
	PlayerColor ownColor;

};

#endif
