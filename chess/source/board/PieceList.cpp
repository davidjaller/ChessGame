#include "PieceList.h"


PieceList::PieceList(PlayerColor ourColor) {
	this->ownColor = ownColor;
}

void PieceList::updateMovement(Move lastMove, const Board* board, PlayerColor lastMoveSide) {

	// Make sure this method is not called until after move has been executed

	bitBoard_t fromBB = 1 << SquareToIndex(lastMove.from);
	bitBoard_t toBB = 1 << SquareToIndex(lastMove.to);
	attacksBB = 0;
	occupiesBB = 0;
	Piece* piece;
	for (iterator_t it = pieces.begin(); it != pieces.end(); it++) {
		piece = &it->second;
		bool reGenerate = false;

		if (lastMove.to == pieces->ownSquare) { // This assumes that piece was allready moved in moveList
			// the piece that moved needs a freach movegeneration
			reGenerate = true;
		}
		else {

			// check the square that was left empty
			if (((piece->blockedOnBB | piece->killesOn) & fromBB) > 0) {
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
		if (regenerate) {
			// for now update for all directions even though we could know which is needed
			piece.updateMovement(board);
		}
		attacksBB |= piece.canMoveToBB | piece.pawnAttacksBB; //for non pawns pawnAttacksBB is 0
		occupiesBB |= 1 << SquareToIndex(piece->ownSquare);  // actually not needed on every iteration but ok for now
	}
}

bitBoard_t PieceList::getAttackingBB() {
	return attacksBB;
}

bitBoard_t PieceList::getOccupiedBB() {
	return occupiedBB;
}

void PieceList::add(Piece piece, Square sq) {
	// Consider constructing the pieces here

	pieces[SquareToIndex(sq)] = piece;
}
void PieceList::movePiece(Square from, Square to) {
	add(pieces[SquareToIndex(from)], to);
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
