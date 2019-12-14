#pragma once
#ifndef _rulesManager_h
#define _rulesManager_h
#include "source/board/Board.h"
#include "source/comon.h"
#include<vector>
#include<set>
class RulesManager
{
	public:
		RulesManager();
		static bool IsLegalPawnMove(const Board* board, Square from, Square to);
		static bool IsLegalBishopMove(const Board* board, Square from, Square to);
		static bool IsLegalRookMove(const Board* board, Square from, Square to);
		static bool IsLegalKingMove(const Board* board, Square from, Square to);
		static bool IsLegalQueenMove(const Board* board, Square from, Square to);
		static bool IsLegalKnightMove(const Board* board, Square from, Square to);
		static bool IsLegalCasteling(const Board* board, Square from, Square to);
		//static bool SquareIsAttacked(const Board* board, Square square, vector<Square>* attackingSquares,
		//	PlayerColor attackingColor, bool stopAtFirst);
		//static bool SquareIsAttacked(const Board* board, Square square, PlayerColor attackingColor);
		//static bool KingIsChecked(const Board* board, vector<Square>* attackingSquares, PlayerColor kingColor);
		static bool IsLegalMove(const Board* board, Square from, Square to);

};

#endif