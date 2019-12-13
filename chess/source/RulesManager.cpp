#include "RulesManager.h"
#include <math.h>
using namespace std;

RulesManager::RulesManager() {
}

bool RulesManager::IsLegalMove(const Position* position, Square from, Square to)
{
	int piece = abs(position->getPieceOnSquare(from));
	switch (piece)
	{
	case PAWN:	if (!IsLegalPawnMove(position, from, to))
		return false;
		break;

	case BISHOP: if (!IsLegalBishopMove(position, from, to))
		return false;
		break;

	case KNIGHT: if (!IsLegalKnightMove(position, from, to))
		return false;
		break;

	case ROOK: if (!IsLegalRookMove(position, from, to))
		return false;
		break;

	case QUEEN: if (!IsLegalQueenMove(position, from, to))
		return false;
		break;

	case KING:		if (!IsLegalKingMove(position, from, to))
		return false;
		break;
	}

	return true;
}

bool RulesManager::IsLegalPawnMove(const Position* position, Square from, Square to)
{
	// Since this method could be used both to check players move and opposite players move
	// it is not enough to check whos turn it is but we need to check the color of the pawn
	int push1, push2, startRank;
	if (position->getPieceOnSquare(from) > 0) {
		push1 = -1;
		push2 = -2;
		startRank = 6;
	}
	else {
		push1 = 1;
		push2 = 2;
		startRank = 1;
	}

	if (position->IsEmptySquare(to))
	{
		if (from.file == to.file)
		{
			if (to.rank - from.rank == push1 
				|| from.rank == startRank && to.rank - from.rank == push2)
			{
				return true;
			}
		}

	}
	else //taking piece
	{
		if (to.rank - from.rank == push1
			&& abs(to.file - from.file) == 1)
		{
			return true;
		}
	}

	return false;
}

bool RulesManager::IsLegalBishopMove(const Position* position, Square from, Square to)
{
	if (abs(to.file - from.file) == abs(to.rank - from.rank))
	{
		int dist = abs(to.file - from.file) - 1;
		int i = 0;
		int j = 0;
		Square sq;

		while (abs(i) < dist)
		{
			if (to.file - from.file > 0)
				i++;
			else
				i--;
			if (to.rank - from.rank > 0)
				j++;
			else
				j--;
			sq.file = from.file + i;
			sq.rank = from.rank + j;
			if (position->getPieceOnSquare(sq) != 0)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool RulesManager::IsLegalRookMove(const Position* position, Square from, Square to)
{
	Square sq;
	if (from.file == to.file || from.rank == to.rank)
	{
		int dist = (abs(to.file - from.file) - 1) * (from.rank == to.rank) + (abs(to.rank - from.rank) - 1) * (from.file == to.file);
		int i = 0;
		int j = 0;
		while (abs(i) < dist && abs(j) < dist)
		{
			if (to.file - from.file > 0)
				i++;
			else if (to.file - from.file < 0)
				i--;
			if (to.rank - from.rank > 0)
				j++;
			else if (to.rank - from.rank < 0)
				j--;
			sq.file = from.file + i;
			sq.rank = from.rank + j;
			if (!position->IsEmptySquare(sq))
			{
				return false;
			}

		}
		return true;
	}
	else
		return false;
}

bool RulesManager::IsLegalKingMove(const Position* position, Square from, Square to){

	if (IsLegalCasteling(position, from, to))
		return true;
	else if (abs(to.file - from.file) + abs(to.rank - from.rank) == 1 || abs(to.file - from.file) == abs(to.rank - from.rank))
		return true;
	else
		return false;
}

bool RulesManager::IsLegalCasteling(const Position* position, Square from, Square to){

	Square sq = to;
	if (from.file == 4 && position->POV(from.rank)) {
		// Long
		if (to.file == 2) {
			if ((to.rank == 0 && position->getCastelingPossible(BLACK_LONG)) ||
				(to.rank == 7 && position->getCastelingPossible(WHITE_LONG))) {
				for (int i = 1; i < 4; i++) {
					sq.file = i;
					if (!position->IsEmptySquare(sq))
						return false;
				}
				for (int i = 2; i < 5; i++) {
					sq.file = i;
					if (SquareIsAttacked(position, sq, getOpposite(position->getTurn())))
						return false;
				}

				return true;
			}
		}
		// short
		else if (to.file == 6) {
			if ((to.rank == 0 && position->getCastelingPossible(BLACK_SHORT)) ||
				(to.rank == 7 && position->getCastelingPossible(WHITE_SHORT))) {
				for (int i = 6; i < 7; i++) {
					sq.file = i;
					if (!position->IsEmptySquare(to))
						return false;
				}
				for (int i = 5; i < 7; i++) {
					sq.file = i;
					if (SquareIsAttacked(position, to, getOpposite(position->getTurn())))
						return false;
				}
				return true;
			}
		}
	}
	
	return false;
}

bool RulesManager::IsLegalQueenMove(const Position* position, Square from, Square to)
{
	if (IsLegalRookMove(position, from, to) || IsLegalBishopMove(position, from, to))
		return true;
	else return false;
}

bool RulesManager::IsLegalKnightMove(const Position* position, Square from, Square to)
{

	int distX = abs(to.file - from.file);
	int distY = abs(to.rank - from.rank);
	if (distX + distY == 3 && distX >= 1 && distY >= 1)
		return true;
	else
		return false;
}

