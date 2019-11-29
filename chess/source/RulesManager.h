#pragma once
#ifndef _rulesManager_h
#define _rulesManager_h
#include "source/game.h"
#include "source/board/Board.h"
class RulesManager
{
	public:
		RulesManager();
		static bool IsLegalPawnMove(Board* board, Square from, Square to);
		static bool IsLegalBishopMove(Board* board, Square from, Square to);
		static bool IsLegalRookMove(Board* board, Square from, Square to);
		static bool IsLegalKingMove(Board* board, Square from, Square to);
		static bool IsLegalQueenMove(Board* board, Square from, Square to);
		static bool IsLegalKnightMove(Board* board, Square from, Square to);
		static bool IsLegalCasteling(Board* board, Square to);
		static bool PieceIsUnderThreat(Board* board, Square pieceSquare, Square* attackingSquare);
		static bool PieceIsUnderThreat(Board* board, Square pieceSquare);
		static bool KingIsChecked(Board* board, Square* attackingSquare);
		static bool IsLegalMove(Board* board, Square from, Square to);

};

#endif