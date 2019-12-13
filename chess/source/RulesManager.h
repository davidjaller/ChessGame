#pragma once
#ifndef _rulesManager_h
#define _rulesManager_h
#include "source/board/position.h"
#include "source/comon.h"
#include "source/board/PieceList.h"
#include<vector>

class RulesManager
{
	public:
		RulesManager();
		static bool IsLegalPawnMove(const Position* position, Square from, Square to);
		static bool IsLegalBishopMove(const Position* position, Square from, Square to);
		static bool IsLegalRookMove(const Position* position, Square from, Square to);
		static bool IsLegalKingMove(const Position* position, Square from, Square to);
		static bool IsLegalQueenMove(const Position* position, Square from, Square to);
		static bool IsLegalKnightMove(const Position* position, Square from, Square to);
		static bool IsLegalCasteling(const Position* position, Square from, Square to);
		static bool IsLegalMove(const Position* position, Square from, Square to);

};

#endif