
#include "source/board/Piece.h"

Piece::Piece(int pieceType) {
	this->pieceType = pieceType;
}

int Piece::getType() {
	return pieceType;
}