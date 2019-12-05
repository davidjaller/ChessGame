#include "RulesManager.h"
#include <math.h>
using namespace std;

RulesManager::RulesManager() {
}

bool RulesManager::IsLegalMove(const Board* board, Square from, Square to)
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

bool RulesManager::IsLegalPawnMove(const Board* board, Square from, Square to)
{
	// Since this method could be used both to check players move and opposite players move
	// it is not enough to check whos turn it is but we need to check the color of the pawn
	int push1, push2, startRank;
	if (board->getPieceOnSquare(from) > 0) {
		push1 = -1;
		push2 = -2;
		startRank = 6;
	}
	else {
		push1 = 1;
		push2 = 2;
		startRank = 1;
	}

	if (board->IsEmptySquare(to))
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

bool RulesManager::IsLegalBishopMove(const Board* board, Square from, Square to)
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
			if (board->getPieceOnSquare(sq) != 0)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool RulesManager::IsLegalRookMove(const Board* board, Square from, Square to)
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

bool RulesManager::IsLegalKingMove(const Board* board, Square from, Square to){

	if (from.file == 4 && board->POV(from.rank) == 0 && board->POV(to.rank) == 0 && (to.file == 2 || to.file == 6))
		// TODO: find out why this check is needed
	{
		if (IsLegalCasteling(board, to))
			return true;
		else
			return false;
	}
	else if (abs(to.file - from.file) + abs(to.rank - from.rank) == 1 || abs(to.file - from.file) == abs(to.rank - from.rank))
		return true;
	else
		return false;
}

bool RulesManager::IsLegalCasteling(const Board* board, Square to){

	Square sq = to;

	// Long
	if (to.file == 2) {
		if ((to.rank == 0 && board->getCastelingPossible(BLACK_LONG)) ||
			(to.rank == 7 && board->getCastelingPossible(WHITE_LONG))) {
			for (int i = 1; i < 4; i++) {
				sq.file = i;
				if (!board->IsEmptySquare(sq))
					return false;
			}
			for (int i = 2; i < 5; i++) {
				sq.file = i;
				if (SquareIsAttacked(board, sq, getOpposite(board->getTurn())))
					return false;
			}

			return true;
		}
	}
	// short
	else if (to.file == 6) {
		if ((to.rank == 0 && board->getCastelingPossible(BLACK_SHORT)) ||
			(to.rank == 7 && board->getCastelingPossible(WHITE_SHORT))) {
			for (int i = 6; i < 7; i++) {
				sq.file = i;
				if (!board->IsEmptySquare(to))
					return false;
			}
			for (int i = 5; i < 7; i++) {
				sq.file = i;
				if (SquareIsAttacked(board, to, getOpposite(board->getTurn())))
					return false;
			}
			return true;
		}
	}
	
	
	return false;

}

bool RulesManager::IsLegalQueenMove(const Board* board, Square from, Square to)
{
	if (IsLegalRookMove(board, from, to) || IsLegalBishopMove(board, from, to))
		return true;
	else return false;
}

bool RulesManager::IsLegalKnightMove(const Board* board, Square from, Square to)
{

	int distX = abs(to.file - from.file);
	int distY = abs(to.rank - from.rank);
	if (distX + distY == 3 && distX >= 1 && distY >= 1)
		return true;
	else
		return false;
}

bool RulesManager::KingIsChecked(const Board* board, vector<Square>* attackingSquares, PlayerColor kingColor)
{
	return SquareIsAttacked(board, board->GetKingPos(kingColor), attackingSquares, getOpposite(kingColor), true);
}

bool RulesManager::SquareIsAttacked(const Board* board, Square square, vector<Square>* attackingSquares, PlayerColor attackingColor, bool stopAtFirst)
{
	// this could also be done by checking diagonal looking for enemy bishop, pawn or queen
	// and sideways looking for caste or queen, and knigh-wise looking for knight,
	// but that is not quicker in all situations
	// What would speed tings up is storing which squares are threatened so that we dont need to call this as often
	attackingSquares->clear();
	Square sq;
	
	set<int> enemySet = board->getAlivePieceSet(attackingColor);
	
	for (set<int>::iterator it = enemySet.begin(); it != enemySet.end(); ++it) {
		Board::IndexToSquare(*it, &sq);
			if (!board->IsEmptySquare(sq) && !board->IsFriendlyPiece(sq))
			{
				if (IsLegalMove(board, sq, square))
				{
					attackingSquares->push_back(sq);
					if (stopAtFirst)
						return true;
				}
			}
		}
	
	return !attackingSquares->empty();
}

// Overloaded method without output pointer
bool RulesManager::SquareIsAttacked(const Board* board, Square square, PlayerColor attackingColor) {

	vector<Square> sqv;
	
	return SquareIsAttacked(board, square, &sqv, attackingColor, true);
}