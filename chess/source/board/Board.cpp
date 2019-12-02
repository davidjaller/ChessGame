
#include "source/board/Board.h"

Board::Board() {};


bool Board::IsEmptySquare(Square sq) const
{
	if (getPieceOnSquare(sq) == 0)
		return true;
	else
		return false;
}

bool Board::IsFriendlyPiece(int piece) const
{
	if ((getTurn() == PlayerColor::WHITE && piece > 0)
		|| (getTurn() == PlayerColor::BLACK && piece < 0))
		return true;
	else
		return false;
}

bool Board::IsFriendlyPiece(Square square) const
{
	int piece = getPieceOnSquare(square);
	return IsFriendlyPiece(piece);
}

void Board::MovePieceKill(Square from, Square to) {

	RemovePiece(to);
	MovePiece(from, to);
}

void Board::MovePieceNoKill(Square from, Square to) {
	MovePiece(from, to);
}

void Board::PromoteQueen(Square square) {

	int piece = getPieceOnSquare(square);
	RemovePiece(square);

	// As long as our whiteAlivePieceIdxs list doesnt 
	// hold the piecetype we dont need to update it here
	if (piece > 0) {
		SetPieceOnSquare(WHITE_QUEEN, square);
	}
	else if (piece < 0)
		SetPieceOnSquare(BLACK_QUEEN, square);
}


void Board::makeMoveFromTo(Square from, Square to)
{
	int fromPiece = getPieceOnSquare(from);
	int toPiece = getPieceOnSquare(to);

	// If rook moves update casteling posablity
	if (fromPiece == WHITE_ROOK && from.y == 0)
		setCastelingNotPossible(WHITE_SHORT);

	else if (fromPiece == WHITE_ROOK && from.x == 7)
		setCastelingNotPossible(WHITE_LONG);

	else if (fromPiece == BLACK_ROOK && from.x == 0)
		setCastelingNotPossible(BLACK_SHORT);

	else if (fromPiece == BLACK_ROOK && from.x == 7)
		setCastelingNotPossible(BLACK_LONG);

	// king move
	else if (abs(fromPiece) == KING)
	{
		// Check if a casteling move and move the rook
		if (from.x == 4 && POV(from.y) == 0 && POV(to.y) == 0 && (to.x == 2 || to.x == 6)) {
			Square rookFrom = from;
			Square rookTo = to;
			if (to.x == 2) { // long
				rookFrom.x = 0;
				rookTo.x = 3;
			}
			else if (to.x == 6) { // short
				rookFrom.x = 7;
				rookTo.x = 5;
			}
			MovePiece(rookFrom, rookTo);
		}

		// if king move then casteling not possible in future
		if (getTurn() == PlayerColor::WHITE)
		{
			setCastelingNotPossible(WHITE_SHORT);
			setCastelingNotPossible(WHITE_LONG);
		}
		else if (getTurn() == PlayerColor::BLACK)
		{
			setCastelingNotPossible(BLACK_SHORT);
			setCastelingNotPossible(BLACK_LONG);
		}
		SetKingPos(to);
	}
	if (!IsEmptySquare(to))
		MovePieceKill(from, to);
	else
		MovePiece(from, to);

	//pawn promotion
	if (abs(fromPiece) == PAWN && (to.y == 0 || to.y == 7))
	{
		PromoteQueen(to);
	}
}

