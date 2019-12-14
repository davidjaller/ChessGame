
#include "source/board/Piece.h"

Piece::Piece(int pieceType, Square ownSquare, PlayerColor ownColor) {

	this->pieceType = pieceType;
	this->ownSquare = ownSquare;
	this->ownColor = ownColor;

	ownPositionBB = squareToBitBoard(ownSquare);
	canMoveToBB = 0;
	blockedOnBB = 0;
	canKillOnBB = 0;
	pawnAttacksBB = 0;
}

Piece::Piece(){}

int Piece::getType() const{
	return pieceType;
}

bool Piece::isSlider() const{
	if (pieceType == ROOK || pieceType == BISHOP || pieceType == QUEEN)
		return true;
	else
		return false;
}

bool Piece::isOurEnemy(Square sq, const Board* board) {
	if ((ownColor == PlayerColor::WHITE && board->getPieceOnSquare(sq) < 0) ||
		(ownColor == PlayerColor::BLACK && board->getPieceOnSquare(sq) > 0))
		return true;
	else
		return false;
}
bool Piece::isOurFriend(Square sq, const Board* board) {
	if ((ownColor == PlayerColor::WHITE && board->getPieceOnSquare(sq) > 0) ||
		(ownColor == PlayerColor::BLACK && board->getPieceOnSquare(sq) < 0))
		return true;
	else
		return false;

}

void Piece::updateMovement(const Board* board) {
	// consider instead having each type be its own class enheriting from Piece

	canMoveToBB = 0; // reset
	blockedOnBB = 0;
	canKillOnBB = 0;
	pawnAttacksBB = 0;

	// this is pseudo legal move generation, moving in to check and if casteling legal is checked later 

	switch (pieceType)
	{
	case PAWN:	 generateForPawn(board);
		break;
	case BISHOP: generateForBishop(board);
		break;
	case KNIGHT: generateForKnight(board);
		break;
	case ROOK:	 generateForRook(board);
		break;
	case QUEEN:  generateForQueen(board);
		break;
	case KING:	 generateForKing(board);
	}
}

void Piece::generateForBishop(const Board* board) {
	vector<array <int, 2>> directions = { {  {-1, -1}, {1, -1}, {1, 1}, {-1, 1} } };
	return generateForSliders(board, directions);
}

void Piece::generateForRook(const Board* board) {

	vector<array <int, 2>> directions = { {  {-1, 0}, {1, 0}, {0, 1}, {0, -1} } };
	return generateForSliders(board, directions);
}

void Piece::generateForQueen(const Board* board) {

	vector<array <int, 2>> directions = { { {-1,0}, {-1, -1}, {0, -1}, {1, -1} ,{1, 0}, {1, 1}, {0, 1}, {-1, 1} } };
	return generateForSliders(board, directions);
}

void Piece::generateForSliders(const Board* board, vector<array<int, 2>> directions) {

	Square to;

	int count = 0;
	for (int i = 0; i < directions.size(); i++) {

		to = ownSquare;
		while (insideBoard(to.rank += directions[i][0], to.file += directions[i][1])) {

			if (isOurFriend(to, board)){
				blockedOnBB |= squareToBitBoard(to);
				break;
			}
			else if (isOurEnemy(to, board)) {
				canKillOnBB |= squareToBitBoard(to);
				break;
			}
			else { //empty square
				canMoveToBB |= squareToBitBoard(to);
			}
		}
	}
}

void Piece::generateForPawn(const Board* board) {

	int push1, push2, startRank;
	
	Square to = ownSquare;
	if (ownColor == PlayerColor::WHITE) { //rank 0 is black(consider changing)
		push1 = -1;
		push2 = -2;
		startRank = 6;
	}
	else {
		push1 = 1;
		push2 = 2;
		startRank = 1;
	}
	// 4 different possibilities for pawns (exluding en pasan for now)

	// push
	
	// one
	to.rank = ownSquare.rank + push1;
	if (to.rank < 8 && to.rank >= 0) {
		if (board->IsEmptySquare(to)) 
			canMoveToBB |= squareToBitBoard(to);
			// two
			to.rank = ownSquare.rank + push2;
			if (to.rank < 8 && to.rank >= 0) {
				if (board->IsEmptySquare(to))
					canMoveToBB |= squareToBitBoard(to);
				else
					blockedOnBB |= squareToBitBoard(to);
			}
		else
			blockedOnBB |= squareToBitBoard(to);
	}
	// kill
	to.rank = ownSquare.rank + push1;
	// left
	to.file = ownSquare.file - 1;
	if (to.file >= 0){
		if (isOurEnemy(to, board)) 
			canKillOnBB |= squareToBitBoard(to);
		else if(board->IsEmptySquare(to))
			pawnAttacksBB |= squareToBitBoard(to);
	}
	// right
	to.file = ownSquare.file + 1;
	if (to.file < 8){
		if (isOurEnemy(to, board))
			canKillOnBB |= squareToBitBoard(to);
		else if (board->IsEmptySquare(to))
			pawnAttacksBB |= squareToBitBoard(to);
	}
}

void Piece::generateForKnight(const Board* board) {

	vector<array <int, 2>> directions = { {  {-1, -2}, {-1, 2}, {1, -2}, {1, 2} , {-2, -1} , {-2, 1} , {2, -1}, {2, 1}} };
	Square to = ownSquare;

	for (int i = 0; i < directions.size(); i++) {

		to.rank = ownSquare.rank + directions[i][0];
		to.file = ownSquare.file + directions[i][1];

		if (insideBoard(to)) {
			if (isOurFriend(to, board))
				blockedOnBB |= squareToBitBoard(to);
			else if (isOurEnemy(to, board))
				canKillOnBB |= squareToBitBoard(to);
			else
				canMoveToBB |= squareToBitBoard(to);
		}
	}
}

void Piece::generateForKing(const Board* board) {

	vector<array <int, 2>> directions = { { {-1,0}, {-1, -1}, {0, -1}, {1, -1} ,{1, 0}, {1, 1}, {0, 1}, {-1, 1} } };
	Square to;

	//Casteling moves handled later when filtering moves
	/*
	to.rank = ownSquare.rank;
	// Long
	to.file = 2;
	canMoveToBB |= squareToBitBoard(to);
	
	// Short
	to.file = 6;
	canMoveToBB |= squareToBitBoard(to); 
	*/
	

	// Normal move
	for (int i = 0; i < directions.size(); i++) {

		to.rank = ownSquare.rank + directions[i][0];
		to.file = ownSquare.file + directions[i][1];
		if (insideBoard(to)) {
			if (isOurFriend(to, board))
				blockedOnBB |= squareToBitBoard(to);
			else if (isOurEnemy(to, board))
				canKillOnBB |= squareToBitBoard(to);
			else
				canMoveToBB |= squareToBitBoard(to);
		}
	}
}