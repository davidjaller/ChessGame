#include "PieceList.h"


PieceList::PieceList(PlayerColor ourColor) {
	this->ownColor = ownColor;
}

void PieceList::updateMovement(Move lastMove, const Board* board, PlayerColor lastMoveSide) {

	// Make sure this method is not called until after move has been executed

	bitBoard_t fromBB = squareToBitBoard(lastMove.from);
	bitBoard_t toBB = squareToBitBoard(lastMove.to);
	attacksBB = 0;
	occupiesBB = 0;
	Piece* piece;
	bool reGenerate;

	for (iterator_t it = pieces.begin(); it != pieces.end(); it++) {
		piece = &it->second;
		reGenerate = false;

		if (lastMove.to == piece->ownSquare) { // This assumes that piece was allready moved in moveList
			// the piece that moved needs a freach movegeneration
			reGenerate = true;
		}
		else {

			// check the square that was left empty
			if (((piece->blockedOnBB | piece->canKillOnBB) & fromBB) > 0) {
				if (piece->isSlider()) {
					// if slider we need to recalculate its movement
					reGenerate = true;
				}
				else {
					// if pawn, knight or king we can just remove blocker and attack if there
					if (piece->getType() == PAWN)
						piece->pawnAttacksBB |= piece->canKillOnBB & fromBB;  // ...and change kill to attack for pawn
					piece->blockedOnBB &= ~fromBB;
					piece->canKillOnBB &= ~fromBB;
					piece->canMoveToBB |= fromBB;

				}
			}
			if (lastMove.kill) {
				// since piece color has changed on the to square we can just toggle blockedOn and canKillOn on the to-square
				piece->blockedOnBB ^= toBB;
				piece->canKillOnBB ^= toBB;
			}
			// Check the to-square, since not kill, it was empty before
			else if ((piece->canMoveToBB & toBB) > 0) {
				if (piece->isSlider()) {
					// if slider we need to recalculate its movement
					reGenerate = true;
				}
				else { // if pawn, knight or king we can handle it quicker
					piece->canMoveToBB &= ~toBB; // we can not move to this square anymore
					piece->pawnAttacksBB &= ~toBB;
					if (lastMoveSide == ownColor)  // was our move
						piece->blockedOnBB |= toBB;
					else
						piece->canKillOnBB |= toBB;
				}

			}
		}
		if (reGenerate) {
			// for now update for all directions even though we could know which is needed
			piece->updateMovement(board);
		}
		attacksBB |= piece->canMoveToBB | piece->pawnAttacksBB; //for non pawns pawnAttacksBB is 0
		occupiesBB |= squareToBitBoard(piece->ownSquare);  // actually not needed on every iteration but ok for now
	}
}

bool PieceList::getPiecesAttackingPiece(Square attackedPieceSquare, list<const Piece*>* attackingPieces) const{
	bool any = false;
	bitBoard_t attackedPieceBB = squareToBitBoard(attackedPieceSquare);
	for (const_iterator_t it = pieces.begin(); it != pieces.end(); it++) {
		if ((it->second.canKillOnBB & attackedPieceBB) != 0) {
			any = true;
			attackingPieces->push_back(&it->second);
		}
	}
	return any;
}

bitBoard_t PieceList::getAttackingBB() const{
	return attacksBB;
}

bitBoard_t PieceList::getOccupiesBB() const{
	return occupiesBB;
}

bool PieceList::getPieceOnSquare(Square square, const Piece* piece) const {
	try {
		piece = &pieces.at(SquareToIndex(square));
		return true;
	}
	catch(...){
		return false;
	}
}

void PieceList::add(int pieceType, Square sq) {
	// Consider constructing the pieces here

	pieces[SquareToIndex(sq)] = Piece(pieceType, sq, ownColor);
}
void PieceList::movePiece(int pieceType, Square from, Square to) {
	add(pieceType, to);
	removePiece(from);
}
void PieceList::removePiece(Square square) {
	pieces.erase(SquareToIndex(square));
}
void PieceList::clearList() {
	pieces.clear();
}
const_iterator_t PieceList::begin() const{
	return pieces.begin();
}
const_iterator_t PieceList::end() const{
	return pieces.end();
}
int PieceList::getPieceType(Square sq) {
	
	return pieces[SquareToIndex(sq)].getType();
}
