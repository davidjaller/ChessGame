
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

bool Board::IsEnemyPiece(Square sq) const
{
	int piece = getPieceOnSquare(sq);
	PlayerColor turn = getTurn();
	if (turn == PlayerColor::WHITE &&  piece < 0
		|| turn == PlayerColor::BLACK && piece > 0)
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
	if (fromPiece == WHITE_ROOK && from.rank == 0)
		setCastelingNotPossible(WHITE_SHORT);

	else if (fromPiece == WHITE_ROOK && from.file == 7)
		setCastelingNotPossible(WHITE_LONG);

	else if (fromPiece == BLACK_ROOK && from.file == 0)
		setCastelingNotPossible(BLACK_SHORT);

	else if (fromPiece == BLACK_ROOK && from.file == 7)
		setCastelingNotPossible(BLACK_LONG);

	// king move
	else if (abs(fromPiece) == KING)
	{
		// Check if a casteling move and move the rook
		if (from.file == 4 && POV(from.rank) == 0 && POV(to.rank) == 0) {
			Square rookFrom = from;
			Square rookTo = to;
			if (to.file == 2) { // long
				rookFrom.file = 0;
				rookTo.file = 3;
				MovePiece(rookFrom, rookTo);
			}
			else if (to.file == 6) { // short
				rookFrom.file = 7;
				rookTo.file = 5;
				MovePiece(rookFrom, rookTo);
			}
			
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
	if (abs(fromPiece) == PAWN && (to.rank == 0 || to.rank == 7))
	{
		PromoteQueen(to);
	}
}

