#include "moveGenerator.h"

//int MoveGenerator::generate(const Position* position, Square from, int piece, list<Move> *moveList) {
//
//	switch (abs(piece))
//	{
//	case PAWN:	return generateForPawn(position, from, piece, moveList);
//
//	case BISHOP: return generateForBishop(position, from, piece, moveList);
//
//	case KNIGHT: return generateForKnight(position, from, piece, moveList);
//
//	case ROOK:	 return generateForRook(position, from, piece, moveList);
//
//	case QUEEN:  return generateForQueen(position, from, piece, moveList);
//
//	case KING:	 return generateForKing(position, from, piece, moveList);
//	}
//
//	return 0;
//}
//
int MoveGenerator::generateAll(const Position* position, list<Move>* moveList) {

	PlayerColor turn = position->getTurn();
	vector<Square> attackingSquares;
	if(position->KingIsChecked(position, &attackingSquares, turn)){
		return generateGetOutOfAttack(position, position->getKingPos(turn), attackingSquares, turn, moveList);
	}

	Square sq;
	int count = 0;
	
	
	//bitboard_t kingMask = getKingMask(castelingRights, getAttacksBB(getOposite(turn)));


	for (const_iterator_t it = position->piecesBegin(turn); it != position->piecesEnd(turn); ++it) {
		Piece piece = it->second; // consider eliminating this copy
		bitBoard_t movement = piece.canMoveToBB;
		if (piece.getType() == KING) {
			//movement &= kingMask;
		}
		else {
			bitBoard_t pinMask = getPinMask(position, piece.ownSquare, position->getKingPos(turn));
			// movement &= pinMask;
		}

	}
	return count;
}





int MoveGenerator::generateGetOutOfAttack(const Position* position, Square attackedSquare, vector<Square> attackingSquares, list<Move>* moveList) {
	// Comonly used for when we are being checked, in order not to have to go trough all pieces
	// moves just to see that we are still checked (could also be usefull for queen attacks)
	return 0;
}


bool MoveGenerator::movingIntoCheck(const Position* position, Move move) {
	// here it would be nice if we knew allready which pieces was pinned to king and can not be moved,
	// the possibly using bitboard mask to filter out moves instead of calling this each time
	Position tempPosition = *position;
	tempPosition.makeMoveFromTo(move.from, move.to);
	vector<Square> sqrs;
	return RulesManager::KingIsChecked(&tempPosition, &sqrs, position->getTurn());
}

bitBoard_t MoveGenerator::getPinMask(const Position* position, Square thisSquare, Square kingSquare) {


	int rankDist = kingSquare.rank - thisSquare.rank;
	int fileDist = kingSquare.rank - thisSquare.rank;
	bitBoard_t pinMask = 0;
	bool isPinned = false;

	// Are we on same ray as king?
	if (fileDist == 0 || rankDist == 0 || fileDist == rankDist) {

		Square direction(0,0); //positive towards king
		if (rankDist > 0)
			direction.rank = 1;
		else if (rankDist < 0)
			direction.rank = -1;
		if (fileDist > 0)
			direction.file = 1;
		else if (fileDist < 0)
			direction.file = -1;

		// First check squares between king and our piece, if anyone there no pinn
		Square sq = thisSquare;
		while ((sq+=direction)!=kingSquare && insideBoard(sq)) {
			//Step towards king along ray 
			if (!position->IsEmptySquare(sq))
				return 0xFFFFFFFF;
			else
				pinMask |= 1 << SquareToIndex(sq);
		}
		
		//If we have not returned it means we are closest to king
		// Now move other direction to see if there is a relevant slider
		sq = thisSquare;
		while (insideBoard(sq-=direction)) {

			int pieceType = abs(position->getPieceOnSquare(sq));

			if (position->IsEmptySquare(sq))
				pinMask |= 1 << SquareToIndex(sq);

			else if (position->IsEnemyPiece(sq) &&
				( ( pieceType == ROOK && (rankDist == 0 || fileDist == 0) ) ||
				(pieceType == BISHOP && rankDist == fileDist) ||
				pieceType == QUEEN)) 
			{
				pinMask |= 1 << SquareToIndex(sq);
				isPinned = true;
				break;
			}
			else // some other piece blocking
				break;
		}
	}

	if (isPinned)
		return pinMask;
	else
		return 0xFFFFFFFF; // if not pinned, all 1's
	
}