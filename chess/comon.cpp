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
bool alignedSquares(Square square1, Square square2, Square* direction){
	int rankDist = square2.rank - square1.rank;
	int fileDist = square2.file - square1.file;
	*direction = Square(0, 0);
	if (fileDist == 0 || rankDist == 0 || fileDist == rankDist) {
		if (rankDist > 0)
			direction->rank = 1;
		else if (rankDist < 0)
			direction->rank = -1;
		if (fileDist > 0)
			direction->file = 1;
		else if (fileDist < 0)
			direction->file = -1;
		return true;
	}
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

int SquareToIndex(Square sq) {
	// 64 squares, index row by row from top left
	// index 0 is file=0, rank=0, index 7 is x7, rank=0, index 63 is file=7, rank=7
	return sq.rank * 8 + sq.file;
}


Square IndexToSquare(int index) {
	// 64 squares, index row by row from top left
	// index 0 is file=0, rank=0, index 7 is x7, rank=0, index 63 is file=7, rank=7
	Square square;
	square.file = index % 8;
	square.rank = index / 8;
	return square;
}

int SquareToIndex(int sqY, int sqX) {
	// 64 squares, index row by row from top left
	// index 0 is file=0, rank=0, index 7 is x7, rank=0, index 63 is file=7, rank=7
	return sqY * 8 + sqX;
}

void bitBoardToSquares(vector<Square>* square_v, bitBoard_t bitBoard) {
	int i = 0;
	while (bitBoard != 0) {
		if ((bitBoard & 1) == 1) {
			square_v->push_back(IndexToSquare(i));
		}
		bitBoard >> 1;
		i++;
	}
}

bitBoard_t squareToBitBoard(Square sq) {
	return bitBoard_t(1) << SquareToIndex(sq);
}
