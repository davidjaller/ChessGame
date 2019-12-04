#pragma once
#ifndef _rulesManager_h
#define _rulesManager_h
#include "source/board/Board.h"
#include<vector>
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
		static bool IsLegalCasteling(const Board* board, Square to);
		static bool PieceIsUnderThreat(const Board* board, Square pieceSquare, vector<Square>* attackingSquares, bool stopAtFirst);
		static bool PieceIsUnderThreat(const Board* board, Square pieceSquare);
		static bool KingIsChecked(const Board* board, vector<Square>* attackingSquares);
		static bool IsLegalMove(const Board* board, Square from, Square to);

};

#endif