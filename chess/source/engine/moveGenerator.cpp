#include "moveGenerator.h"
////////////////////////
// constants:
////////////////////////

const bitBoard_t WHITE_SHORT_NO_OCC_BB = squareToBitBoard(Square(7, 5)) | squareToBitBoard(Square(7, 6));
const bitBoard_t WHITE_SHORT_NO_ATT_BB = squareToBitBoard(Square(7, 4)) | squareToBitBoard(Square(7, 5)) | squareToBitBoard(Square(7, 6));
const bitBoard_t WHITE_SHORT_BB = squareToBitBoard(Square(7, 6));

const bitBoard_t WHITE_LONG_NO_OCC_BB = squareToBitBoard(Square(7, 1)) | squareToBitBoard(Square(7, 2)) | squareToBitBoard(Square(7, 3));
const bitBoard_t WHITE_LONG_NO_ATT_BB = squareToBitBoard(Square(7, 2)) | squareToBitBoard(Square(7, 3)) | squareToBitBoard(Square(7, 4));
const bitBoard_t WHITE_LONG_BB = squareToBitBoard(Square(7, 2));

const bitBoard_t BLACK_SHORT_NO_OCC_BB = squareToBitBoard(Square(0, 5)) | squareToBitBoard(Square(7, 6));
const bitBoard_t BLACK_SHORT_NO_ATT_BB = squareToBitBoard(Square(0, 4)) | squareToBitBoard(Square(7, 5)) | squareToBitBoard(Square(7, 6));
const bitBoard_t BLACK_SHORT_BB = squareToBitBoard(Square(0, 6));

const bitBoard_t BLACK_LONG_NO_OCC_BB = squareToBitBoard(Square(0, 1)) | squareToBitBoard(Square(0, 2)) | squareToBitBoard(Square(0, 3));   //0x0B  ...001110 
const bitBoard_t BLACK_LONG_NO_ATT_BB = squareToBitBoard(Square(0, 2)) | squareToBitBoard(Square(0, 3)) | squareToBitBoard(Square(0, 4)); //0x1C  ...011100
const bitBoard_t BLACK_LONG_BB = squareToBitBoard(Square(0, 2));



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
int MoveGenerator::generateMoves(const Position* position, list<Move>* moveList, bool generateCaptures) {

	// Here we go from pseudo legal move bitBoards to legal Move structs and append them to moveList 
	// That is we now consider also making sure we are not in check after making the move
	// If moveList is empty, then 

	bitBoard_t movementBB;

	PlayerColor turn = position->getTurn();
	list<const Piece*> attackingPieces;
	bool inCheck = position->KingIsChecked(&attackingPieces, turn);

	if (attackingPieces.size() > 1) {
		// if two or more attackers, only moving king can help us, no need to loop trough all pieces
		Piece* king;
		position->getPieceOnSquare(position->getKingPos(turn), turn, king);
		if (generateCaptures) 
			movementBB = king->canKillOnBB;
		else
			movementBB = king->canMoveToBB;
		movementBB &= ~position->getAttackedSquaresBB(getOpposite(turn)) 
			& ~getKingShadowsBB(attackingPieces, turn, position->getKingPos(turn));
		return bitBoardToMoves(movementBB, moveList, king->ownSquare, generateCaptures);
	}

	int count = 0;

	for (const_iterator_t it = position->piecesBegin(turn); it != position->piecesEnd(turn); ++it) {
		Piece piece = it->second; // consider eliminating this copy

		if (generateCaptures) // this is so that we can search captures first
			movementBB = piece.canKillOnBB;
		else
			movementBB = piece.canMoveToBB;

		if (piece.getType() == KING) {
			if (inCheck) {
				// we dont need condider casteling. we do need to consider the kings shadow from an attacking slider.
				movementBB &= ~position->getAttackedSquaresBB(getOpposite(turn)) 
					& ~getKingShadowsBB(attackingPieces, turn, position->getKingPos(turn));
			}
			else {
				movementBB |= getCastellingBB(position, turn);
				movementBB &= ~position->getAttackedSquaresBB(getOpposite(turn)); // cant move to attacked square
			}
		}
		else {
			if (inCheck) {
				// we need a blockAttackerMask and a captureAttackerMask
				movementBB &= getBlockAttackerBB(attackingPieces, turn, position->getKingPos(turn)) 
					| getCaptureAttackerBB(attackingPieces);
			}
			else {				
				movementBB &= getPinMask(position, piece.ownSquare, position->getKingPos(turn));;
			}
		}
		count += bitBoardToMoves(movementBB, moveList, piece.ownSquare, generateCaptures);
	}
	return count;
}


int MoveGenerator::bitBoardToMoves(bitBoard_t movementBB, list<Move>* moveList, Square fromSquare, bool kill) {
	int i = 0;
	int count = 0;
	while (movementBB != 0) {
		if ((movementBB & 1) == 1) {
			Move move;
			move.from = fromSquare;
			move.to = IndexToSquare(i);
			move.kill = kill;
			moveList->push_back(move);
		}
		movementBB >> 1;
		i++;
	}
	return count;
}

bitBoard_t MoveGenerator::getCastellingBB(const Position* position, PlayerColor turn) {

	bitBoard_t castellingBB = 0;
	bitBoard_t occupiedSquares = position->getOccupiedSquaresBB(PlayerColor::WHITE) &
		position->getOccupiedSquaresBB(PlayerColor::BLACK);

	if (turn == PlayerColor::WHITE) {
		bitBoard_t attackedSquares = position->getAttackedSquaresBB(PlayerColor::BLACK);
		if (position->getCastelingPossible(WHITE_SHORT) &&
			(WHITE_SHORT_NO_OCC_BB & occupiedSquares == 0) &&
			(WHITE_SHORT_NO_ATT_BB & attackedSquares == 0)) {
			castellingBB |= WHITE_SHORT_BB;
		}
		if (position->getCastelingPossible(WHITE_LONG) &&
			((WHITE_LONG_NO_OCC_BB & occupiedSquares) == 0) &&
			((WHITE_LONG_NO_ATT_BB & attackedSquares) == 0)) {
			castellingBB |= WHITE_LONG_BB;
		}
		else {
			bitBoard_t attackedSquares = position->getAttackedSquaresBB(PlayerColor::WHITE);
			if (position->getCastelingPossible(BLACK_SHORT) &&
				((BLACK_SHORT_NO_OCC_BB & occupiedSquares) == 0) &&
				((BLACK_SHORT_NO_ATT_BB & attackedSquares) == 0)) {
				castellingBB |= BLACK_SHORT_BB;
			}
			if (position->getCastelingPossible(BLACK_LONG) &&
				((BLACK_LONG_NO_OCC_BB & occupiedSquares) == 0) &&
				((BLACK_LONG_NO_ATT_BB & attackedSquares) == 0)) {
				castellingBB |= BLACK_LONG_BB;
			}

		}
	}
	return castellingBB;
}

bitBoard_t MoveGenerator::getKingShadowsBB(list<const Piece*> attackingPieces, PlayerColor turn, Square kingSquare)
{
	bitBoard_t shadowBB = 0;

	for (list<const Piece*>::const_iterator it  = attackingPieces.begin(); it != attackingPieces.end(); ++it) {
		if ((*it)->isSlider()) {
			Square attackerSq = (*it)->ownSquare;
			Square direction;
			if (alignedSquares(attackerSq, kingSquare, &direction)) 
			{
				shadowBB |= squareToBitBoard(kingSquare + direction);
			}
		}
	}
	return shadowBB;
}

	bitBoard_t MoveGenerator::getBlockAttackerBB(list<const Piece*> attackingPieces, PlayerColor turn, Square kingSquare) {

		return (bitBoard_t)0;
	}

	bitBoard_t MoveGenerator::getCaptureAttackerBB(list<const Piece*> attackingPieces)
	{
		bitBoard_t bb = 0;
		for (list<const Piece*>::const_iterator it = attackingPieces.begin(); it != attackingPieces.end(); ++it) 
		{
			bb |= squareToBitBoard((*it)->ownSquare);
		}
		return bb;
	}

	//
	//bool MoveGenerator::IsLegalCasteling(const Position* position, Square from, Square to) {
	//
	//	Square sq = to;
	//	if (from.file == 4 && position->POV(from.rank)) {
	//		// Long
	//		if (to.file == 2) {
	//			if ((to.rank == 0 && position->getCastelingPossible(BLACK_LONG)) ||
	//				(to.rank == 7 && position->getCastelingPossible(WHITE_LONG))) {
	//				for (int i = 1; i < 4; i++) {
	//					sq.file = i;
	//					if (!position->IsEmptySquare(sq))
	//						return false;
	//				}
	//				for (int i = 2; i < 5; i++) {
	//					sq.file = i;
	//					if (position->SquareIsAttacked(position, sq, getOpposite(position->getTurn())))
	//						return false;
	//				}
	//
	//				return true;
	//			}
	//		}
	//		// short
	//		else if (to.file == 6) {
	//			if ((to.rank == 0 && position->getCastelingPossible(BLACK_SHORT)) ||
	//				(to.rank == 7 && position->getCastelingPossible(WHITE_SHORT))) {
	//				for (int i = 6; i < 7; i++) {
	//					sq.file = i;
	//					if (!position->IsEmptySquare(to))
	//						return false;
	//				}
	//				for (int i = 5; i < 7; i++) {
	//					sq.file = i;
	//					if (position->SquareIsAttacked(position, to, getOpposite(position->getTurn())))
	//						return false;
	//				}
	//				return true;
	//			}
	//		}
	//	}
	//
	//	return false;
	//}



	//bool MoveGenerator::movingIntoCheck(const Position* position, Move move) {
	//	// here it would be nice if we knew allready which pieces was pinned to king and can not be moved,
	//	// the possibly using bitboard mask to filter out moves instead of calling this each time
	//	Position tempPosition = *position;
	//	tempPosition.makeMoveFromTo(move.from, move.to);
	//	vector<Square> sqrs;
	//	return RulesManager::KingIsChecked(&tempPosition, &sqrs, position->getTurn());
	//}


		bitBoard_t MoveGenerator::getPinMask(const Position * position, Square thisSquare, Square kingSquare) { 

		//bitBoard_t pinMask = 0;
		//bool isPinned = false;

		//// Are we on same ray as king?
		//Square direction;
		//if (alignedSquares(thisSquare, kingSquare, &direction)) {

		//	// First check squares between king and our piece, if anyone there no pinn
		//	Square sq = thisSquare;
		//	while ((sq += direction) != kingSquare && insideBoard(sq)) {
		//		//Step towards king along ray 
		//		if (!position->IsEmptySquare(sq))
		//			return 0xFFFFFFFF;
		//		else
		//			pinMask |= 1 << SquareToIndex(sq);
		//	}

		//	//If we have not returned it means we are closest to king
		//	// Now move other direction to see if there is a relevant slider
		//	sq = thisSquare;
		//	while (insideBoard(sq -= direction)) {

		//		int pieceType = abs(position->getBoard()->getPieceOnSquare(sq));

		//		if (position->IsEmptySquare(sq))
		//			pinMask |= 1 << SquareToIndex(sq);

		//		else if (position->IsEnemyPiece(sq) &&
		//			((pieceType == ROOK && (direction.rank != direction.file)) ||
		//			(pieceType == BISHOP && direction.rank == direction.file) ||
		//				pieceType == QUEEN))
		//		{
		//			pinMask |= 1 << SquareToIndex(sq);
		//			isPinned = true;
		//			break;
		//		}
		//		else // some other piece blocking
		//			break;
		//	}
		//}

		//if (isPinned)
		//	return pinMask;
		//else
			return 0xFFFFFFFF; // if not pinned, all 1's

	}
