#pragma once
#ifndef _moveGenerator_h
#define _moveGenerator_h
#include "source/board/position.h"
#include "source/RulesManager.h"
#include"source/comon.h"
#include<vector>
#include<list>
#include "source/board/PieceList.h"

class MoveGenerator {
public:
	static int generate(const Position* position, Square from, int Piece, list<Move> *moveList);

	static int generateAll(const Position* position, list<Move>* moveList);
private:
	static int generateForPawn(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForBishop(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForKnight(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForRook(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForQueen(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForKing(const Position* position, Square from, int piece, list<Move> *moveList);

	static int generateForSliders(const Position* position, Square from, int piece, list<Move>* moveList, vector<array<int, 2>> directions);

	static int generateGetOutOfAttack(const Position* position, Square attackedSquare, vector<Square> attackingSquares, list<Move>* moveList);
	
	static bool movingIntoCheck(const Position* position, Move move);

	static bitBoard_t getPinMask(const Position* position, Square from, Square kingSquare);

};

#endif