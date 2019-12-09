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

void printMove(Move move) {
	cout << moveToStr(move) << endl;
}

void printMoveScore(Move move, float score) {
	cout << "move: " << moveToStr(move) << " score: " << score << endl;
}

string moveToStr(Move move) {
	string pieceStr;
	switch (abs(move.piece)) {
	case PAWN: pieceStr = "";
		break;
	case BISHOP: pieceStr = "B";
		break;
	case KNIGHT: pieceStr = "Kn";
		break;
	case ROOK:	 pieceStr = "R";
		break;
	case QUEEN:  pieceStr = "Q";
		break;
	case KING:	pieceStr = "K";
		break;
	}

	char fileChar = (char) move.to.file + 'a'; //97
	char rankChar = '8' - (char)move.to.rank;
	if (move.kill)
		return pieceStr + 'x' + fileChar + rankChar;
	else
		return pieceStr + fileChar + rankChar;
}