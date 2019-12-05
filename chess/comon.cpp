#include "source/comon.h"


bool insideBoard(Square square) {
	if (square.rank >= 0 && square.rank < 8 && square.file >= 0 && square.file < 8) 
		return true;
	else 
		return false;
}

bool insideBoard(int rank, int file) {
	if (rank >= 0 && rank < 8 && file >= 0 && file < 8)
		return true;
	else
		return false;
}

bool isSliderPiece(int piece) {
	if (abs(piece) == QUEEN || abs(piece) == ROOK || abs(piece) == BISHOP)
		return true;
	else
		return false;
}

PlayerColor getOpposite(PlayerColor color) {
	if (color == PlayerColor::WHITE)
		return PlayerColor::BLACK;
	if (color == PlayerColor::BLACK)
		return PlayerColor::WHITE;
}