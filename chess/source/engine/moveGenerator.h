#pragma once
#ifndef _moveGenerator_h
#define _moveGenerator_h
#include "source/board/Board.h"
#include "source/RulesManager.h"
#include"source/comon.h"
#include<vector>
#include<list>

class MoveGenerator {
public:
	static int generate(const Board* board, Square from, int Piece, list<Move> *moveList);

	static int generateAll(const Board* board, list<Move>* moveList);
private:
	static int generateForPawn(const Board* board, Square from, int piece, list<Move> *moveList);

	static int generateForBishop(const Board* board, Square from, int piece, list<Move> *moveList);

	static int generateForKnight(const Board* board, Square from, int piece, list<Move> *moveList);

	static int generateForRook(const Board* board, Square from, int piece, list<Move> *moveList);

	static int generateForQueen(const Board* board, Square from, int piece, list<Move> *moveList);

	static int generateForKing(const Board* board, Square from, int piece, list<Move> *moveList);

	static int generateForSliders(const Board* board, Square from, int piece, list<Move>* moveList, vector<array<int, 2>> directions);

	static int generateGetOutOfAttack(const Board* board, Square attackedSquare, vector<Square> attackingSquares, list<Move>* moveList);
	
	static bool movingIntoCheck(const Board* board, Move move);

	

};

#endif