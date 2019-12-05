#include "moveGenerator.h"

int MoveGenerator::generate(const Board* board, Square from, int piece, list<Move> *moveList) {

	switch (piece)
	{
	case PAWN:	return generateForPawn(board, from, piece, moveList);

	case BISHOP: return generateForBishop(board, from, piece, moveList);

	case KNIGHT: return generateForKnight(board, from, piece, moveList);

	case ROOK:	 return generateForRook(board, from, piece, moveList);

	case QUEEN:  return generateForQueen(board, from, piece, moveList);

	case KING:	 return generateForKing(board, from, piece, moveList);
	}

	return 0;
}

int MoveGenerator::generateForBishop(const Board* board, Square from, int piece, list<Move> *moveList) {
	vector<array <int, 2>> directions = { {  {-1, -1}, {1, -1}, {1, 1}, {-1, 1} } };
		return generateForSliders(board, from, piece, moveList, directions);
}

int MoveGenerator::generateForRook(const Board* board, Square from, int piece, list<Move> *moveList) {

	vector<array <int, 2>> directions = { {  {-1, 0}, {1, 0}, {0, 1}, {0, -1} } };
	return generateForSliders(board, from, piece, moveList, directions);
}

int MoveGenerator::generateForQueen(const Board* board, Square from, int piece, list<Move> *moveList) {

	vector<array <int, 2>> directions = { { {-1,0}, {-1, -1}, {0, -1}, {1, -1} ,{1, 0}, {1, 1}, {0, 1}, {-1, 1} } };
	return generateForSliders(board, from, piece, moveList, directions);
}

int MoveGenerator::generateForSliders(const Board* board, Square from, int piece, list<Move>* moveList, vector<array<int, 2>> directions) {

	Move move;
	move.from = from;
	move.piece = piece;

	int count = 0;
	for (int i = 0; i < directions.size(); i++) {

		move.to = from;
		while (insideBoard(move.to.rank += directions[i][0], move.to.file += directions[i][1])) {

			if (board->IsFriendlyPiece(move.to) || movingIntoCheck(board, move))
				break;
			else if (board->IsEnemyPiece(move.to)) {
				move.kill = true;
				moveList->push_back(move);
				count++;
				break;
			}
			else { //empty square
				move.kill = false;
				moveList->push_back(move);
				count++;
			}
		}
	}
	return count;
}


int MoveGenerator::generateForPawn(const Board* board, Square from, int piece, list<Move>* moveList) {

	int push1, push2, startRank;
	int count = 0;
	Move move;
	move.from = from;
	move.to = from;
	if (piece > 0) {
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
	move.piece = piece;

	// push
	move.kill = false;
	// one
	move.to.rank = from.rank + push1;
	if (move.to.rank < 8 && move.to.rank >= 0) {
		if (board->IsEmptySquare(move.to)
			&& !movingIntoCheck(board, move)) {
			count++;
			moveList->push_back(move);
		}
		// two
		move.to.rank = move.from.rank + push2;
		if (move.to.rank < 8 && move.from.rank >= 0
			&& board->IsEmptySquare(move.to)
			&& !movingIntoCheck(board, move)) {
			count++;
			moveList->push_back(move);
		}
	}
	// kill
	move.kill = true;
	move.to.rank = move.from.rank + push1;
	// left
	move.to.file = move.from.file - 1;
	if (move.to.file >= 0
		&& board->IsEnemyPiece(move.to)
		&& !movingIntoCheck(board, move)) {
		count++;
		moveList->push_back(move);
	}
	// right
	move.to.file = from.file + 1;
	if (move.to.file < 8
		&& board->IsEnemyPiece(move.to)
		&& !movingIntoCheck(board, move)) {
		count++;
		moveList->push_back(move);
	}

	return count;
}

int MoveGenerator::generateForKnight(const Board* board, Square from, int piece, list<Move>* moveList) {

	vector<array <int, 2>> directions = { {  {-1, -2}, {-1, 2}, {1, -2}, {1, 2} , {-2, -1} , {-2, 1} , {2, -1}, {2, 1}} };
	Move move;
	move.piece = piece;
	move.from = from;

	int count = 0;
	for (int i = 0; i < directions.size(); i++) {

		move.to.rank = from.rank + directions[i][0];
		move.to.file = from.file + directions[i][1];

		if (insideBoard(move.to)) {
			if (!board->IsFriendlyPiece(move.to)) {
				if (!movingIntoCheck(board, move)) {
					if (board->IsEnemyPiece(move.to))
						move.kill = true;
					else
						move.kill = false;
					moveList->push_back(move);
					count++;
				}
			}
		}
	}
	return count;
}

int MoveGenerator::generateForKing(const Board* board, Square from, int piece, list<Move> *moveList) {

	vector<array <int, 2>> directions = { { {-1,0}, {-1, -1}, {0, -1}, {1, -1} ,{1, 0}, {1, 1}, {0, 1}, {-1, 1} } };
	Move move;
	move.piece = piece;
	move.from = from;
	int count = 0;

	//Casteling move
	move.to.rank = from.rank;
	// Long
	move.to.file = 2;
	if (RulesManager::IsLegalCasteling(board, move.to)) {
		moveList->push_back(move);
		count++;
	}
	// Short
	move.to.file = 6;
	if (RulesManager::IsLegalCasteling(board, move.to)) {
		moveList->push_back(move);
		count++;
	}

	// Normal move
	for (int i = 0; i < directions.size(); i++) {

		move.to.rank = from.rank + directions[i][0];
		move.to.file = from.file + directions[i][1];
		if(insideBoard(move.to)){
			if (!board->IsFriendlyPiece(move.to)) {
				if (!RulesManager::SquareIsAttacked(board, move.to, getOpposite(board->getTurn()))){
					if (board->IsEnemyPiece(move.to)) 
						move.kill = true;
					else
						move.kill = false;
					moveList->push_back(move);
					count++;
				}
			}
		}
	}

	return count;
}



int MoveGenerator::generateGetOutOfAttack(const Board* board, Square attackedSquare, vector<Square> attackingSquares, list<Move>* moveList) {
	// Comonly used for when we are being checked, in order not to have to go trough all pieces
	// moves just to see that we are still checked (could also be usefull for queen attacks)
	return 0;
}


bool MoveGenerator::movingIntoCheck(const Board* board, Move move) {
	// here it would be nice if we knew allready which pieces was pinned to king and can not be moved,
	// the possibly using bitboard mask to filter out moves instead of calling this each time
	Board tempBoard = *board;
	tempBoard.makeMoveFromTo(move.from, move.to);
	vector<Square> sqrs;
	return RulesManager::KingIsChecked(&tempBoard, &sqrs, board->getTurn());
}