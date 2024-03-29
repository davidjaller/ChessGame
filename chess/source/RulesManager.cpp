#include "RulesManager.h"
#include <math.h>
using namespace std;

RulesManager::RulesManager() {
}

bool RulesManager::IsLegalMove(Board* board, Square from, Square to)
{
	int piece = abs(board->getPieceOnSquare(from));
	switch (piece)
	{
	case PAWN:	if (!IsLegalPawnMove(board, from, to))
		return false;
		break;

	case BISHOP: if (!IsLegalBishopMove(board, from, to))
		return false;
		break;

	case KNIGHT: if (!IsLegalKnightMove(board, from, to))
		return false;
		break;

	case ROOK: if (!IsLegalRookMove(board, from, to))
		return false;
		break;

	case QUEEN: if (!IsLegalQueenMove(board, from, to))
		return false;
		break;

	case KING:		if (!IsLegalKingMove(board, from, to))
		return false;
		break;
	}

	return true;
}

bool RulesManager::IsLegalPawnMove(Board* board, Square from, Square to)
{
	if (board->IsEmptySquare(to))
	{
		if (from.x == to.x)
		{
			int a = abs(from.y - to.y);
			int b = (board->POV(from.y));
			if (abs(from.y - to.y) == 1 
				|| ((board->POV(from.y) == 1 && abs(from.y - to.y) == 2)))
			{
				return true;
			}
		}

	}
	else //taking piece
	{
		if (board->POV(to.y) - board->POV(from.y) == 1 
			&& abs(board->POV(to.x) - board->POV(from.x)) == 1)
		{
			return true;
		}
	}

	return false;
}

bool RulesManager::IsLegalBishopMove(Board* board, Square from, Square to)
{
	if (abs(to.x - from.x) == abs(to.y - from.y))
	{
		int dist = abs(to.x - from.x) - 1;
		int i = 0;
		int j = 0;
		Square sq;

		while (abs(i) < dist)
		{
			if (to.x - from.x > 0)
				i++;
			else
				i--;
			if (to.y - from.y > 0)
				j++;
			else
				j--;
			sq.x = from.x + i;
			sq.y = from.y + j;
			if (board->getPieceOnSquare(sq) != 0)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool RulesManager::IsLegalRookMove(Board* board, Square from, Square to)
{
	Square sq;
	if (from.x == to.x || from.y == to.y)
	{
		int dist = (abs(to.x - from.x) - 1) * (from.y == to.y) + (abs(to.y - from.y) - 1) * (from.x == to.x);
		int i = 0;
		int j = 0;
		while (abs(i) < dist && abs(j) < dist)
		{
			if (to.x - from.x > 0)
				i++;
			else if (to.x - from.x < 0)
				i--;
			if (to.y - from.y > 0)
				j++;
			else if (to.y - from.y < 0)
				j--;
			sq.x = from.x + i;
			sq.y = from.y + j;
			if (board->getPieceOnSquare(sq) != 0)
			{
				return false;
			}

		}
		return true;
	}
	else
		return false;
}

bool RulesManager::IsLegalKingMove(Board* board, Square from, Square to)
{
	if (from.x == 4 && board->POV(from.y) == 0 && board->POV(to.y) == 0 && (to.x == 2 || to.x == 6))
	{
		if (IsLegalCasteling(board, to))
			return true;
		else
			return false;
	}
	else if (abs(to.x - from.x) + abs(to.y - from.y) == 1 || abs(to.x - from.x) == abs(to.y - from.y))
		return true;
	else
		return false;
}

bool RulesManager::IsLegalCasteling(Board* board, Square to)
{
	Square sq = to;
	if (to.y == 0 && to.x == 2)
	{
		if (board->getCastelingPossible(BLACK_LONG)) {
			sq.y = to.y;
			for (int i = 1; i < 4; i++) {
				sq.x = i;
				if (!board->IsEmptySquare(to))
					return false;
			}
			for (int i = 0; i < 5; i++) {
				sq.x = i;
				if (PieceIsUnderThreat(board, sq))
					return false;
			}

			return true;
		}
	}
	else if (to.y == 0 && to.x == 6)
	{
		if (board->getCastelingPossible(BLACK_SHORT)) {
			for (int i = 6; i < 7; i++) {
				sq.x = i;
				if (!board->IsEmptySquare(to))
					return false;
			}
			for (int i = 5; i < 8; i++) {
				sq.x = i;
				if (PieceIsUnderThreat(board, to))
					return false;
			}
			return true;
		}
	}
	else if (to.y == 7 && to.x == 2)
	{
		if (board->getCastelingPossible(WHITE_LONG)) {
			for (int i = 0; i < 5; i++) {
				sq.x = i;
				if (PieceIsUnderThreat(board, to))
					return false;
			}
			for (int i = 1; i < 4; i++) {
				sq.x = i;
				if (!board->IsEmptySquare(to))
					return false;
			}

			return true;
		}
	}
	else if (to.y == 7 && to.x == 6)
	{
		if (board->getCastelingPossible(WHITE_SHORT)) {
			for (int i = 6; i < 7; i++) {
				sq.x = i;
				if (!board->IsEmptySquare(to))
					return false;
			}
			for (int i = 5; i < 8; i++) {
				sq.x = i;
				if (PieceIsUnderThreat(board, to))
					return false;
			}
			return true;
		}
	}

	return false;

}

bool RulesManager::IsLegalQueenMove(Board* board, Square from, Square to)
{
	if (IsLegalRookMove(board, from, to) || IsLegalBishopMove(board, from, to))
		return true;
	else return false;
}

bool RulesManager::IsLegalKnightMove(Board* board, Square from, Square to)
{

	int distX = abs(to.x - from.x);
	int distY = abs(to.y - from.y);
	if (distX + distY == 3 && distX >= 1 && distY >= 1)
		return true;
	else
		return false;
}

bool RulesManager::KingIsChecked(Board* board, Square* attackingSquare)
{
	return PieceIsUnderThreat(board, board->GetKingPos(Game::getPlayer()), attackingSquare);
}

bool RulesManager::PieceIsUnderThreat(Board* board, Square pieceSquare, Square* attackingSqaure)
{
	// this can be optimized, by checking diagonal looking for enemy bishop, pawn or queen
	// and sideways looking for caste or queen, and knigh-wise looking for knight,
	// though this is quick enough for now
	Square sq;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			sq.x = i;
			sq.y = j;
			if (!board->IsEmptySquare(sq) && !board->IsFriendlyPiece(sq))
			{
				if (IsLegalMove(board, sq, pieceSquare))
				{
					*attackingSqaure = sq;
					return true;
				}
			}
		}
	}
	return false;
}

// Overloaded method without output pointer
bool RulesManager::PieceIsUnderThreat(Board* board, Square pieceSquare) {

	Square sq;
	
	return PieceIsUnderThreat(board, pieceSquare, &sq);
}