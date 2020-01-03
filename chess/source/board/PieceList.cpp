#include "PieceList.h"

PieceList::PieceList(PlayerColor ourColor) 
{
	this->ownColor = ourColor;
}

void PieceList::updateAllMovements(const Board* board)
{
	attacksBB = 0;
	occupiesBB = 0;

	for (iterator_t it = pieces.begin(); it != pieces.end(); it++) 
	{
		Piece* piece = &it->second;
		piece->updateMovement(board);

		if (abs(piece->type) == PAWN)
			attacksBB |= piece->canKillOnBB | piece->pawnAttacksBB;
		else
			attacksBB |= piece->canKillOnBB | piece->canMoveToBB | piece->blockedOnBB;

		occupiesBB |= squareToBitBoard(piece->ownSquare);
	}
}

void PieceList::updateMovement(Move lastMove, const Board* board, PlayerColor lastMoveSide) 
{
	// Make sure this method is not called until after move has been executed, 
	// but allways before detecting check and generating next move  
	// Inheritence should be quite usefull here and we could move most of this code into the inheriting piece classes
	// and perhabs also the legal filtering and movegenereration

	bitBoard_t fromBB = squareToBitBoard(lastMove.from);
	bitBoard_t toBB = squareToBitBoard(lastMove.to);
	attacksBB = 0;
	occupiesBB = 0;
	Piece* piece;
	bool reGenerate;

	for (iterator_t it = pieces.begin(); it != pieces.end(); it++)
	{
		piece = &it->second;
		reGenerate = false;

		if (lastMove.to == piece->ownSquare)
		{
			// the piece that moved needs a frech movegeneration
			reGenerate = true;
		}
		else if (abs(piece->type) == PAWN)
		{
			// pawns behave quite different from rest, a pawn that is in any way affected we give a new update
			if (((piece->blockedOnBB | piece->canMoveToBB | piece->canKillOnBB | piece->pawnAttacksBB) & (fromBB | toBB)) > 0)
				reGenerate = true;
		}
		else
		{
			// check the square that was left empty
			if (((piece->blockedOnBB | piece->canKillOnBB) & fromBB) > 0)
			{
				if (piece->type == WHITE_ROOK)
					int a = 1;
				if (piece->isSlider())
				{
					// if slider we need to recalculate its movement
					reGenerate = true;
				}
				else
				{
					//if knight or king we can just remove blocker or kill if there
					piece->blockedOnBB &= ~fromBB;
					piece->canKillOnBB &= ~fromBB;
					piece->canMoveToBB |= fromBB;

				}
			}
			// Check the to-square,
			if (lastMove.kill)
			{
				// since piece color has changed on the to square we can just toggle between blockedOn and canKillOn on the to-square
				bitBoard_t blockedOnTemp = piece->blockedOnBB;
				piece->blockedOnBB ^= toBB & piece->canKillOnBB;
				piece->canKillOnBB ^= toBB & blockedOnTemp;

			}
			//if not kill it was empty before
			else if ((piece->canMoveToBB & toBB) > 0)
			{
				if (piece->isSlider())
				{
					// if slider we need to recalculate its movement
					reGenerate = true;
				}
				else
				{ // if knight or king we can handle it quicker
					piece->canMoveToBB &= ~toBB; // we can not move to this square anymore
					if (lastMoveSide == ownColor)
						piece->blockedOnBB |= toBB;
					else
						piece->canKillOnBB |= toBB;
				}
			}
		}
		if (reGenerate)
		{
			// for now update for all directions even though we could know which is needed
			piece->updateMovement(board);
		}
		if (abs(piece->type) == PAWN)
		{
			attacksBB |= piece->canKillOnBB | piece->pawnAttacksBB;
		}
		else 
		{
			attacksBB |= piece->canKillOnBB | piece->canMoveToBB | piece->blockedOnBB;
		}
		occupiesBB |= squareToBitBoard(piece->ownSquare);  // actually not needed on every iteration but ok for now
	}
}

bool PieceList::getPiecesAttackingPiece(Square attackedPieceSquare, list<const Piece*>* attackingPieces) const
{
	bool any = false;
	bitBoard_t attackedPieceBB = squareToBitBoard(attackedPieceSquare);

	for (const_iterator_t it = pieces.begin(); it != pieces.end(); it++) 
	{
		if ((it->second.canKillOnBB & attackedPieceBB) != 0) 
		{
			any = true;
			attackingPieces->push_back(&it->second);
		}
	}
	return any;
}

bitBoard_t PieceList::getAttackingBB() const
{
	return attacksBB;
}

bitBoard_t PieceList::getOccupiesBB() const
{
	return occupiesBB;
}

const Piece* PieceList::getPieceOnSquare(Square square) const
{
	try {
		return &pieces.at(SquareToIndex(square));
	}
	catch(...){
		return NULL;
	}
}

void PieceList::add(int pieceType, Square sq) 
{	
	pieces[SquareToIndex(sq)] = Piece(pieceType, sq, ownColor);
}
void PieceList::movePiece(Move move) 
{
	if ((move.piece < 0 && ownColor == PlayerColor::WHITE)
		|| (move.piece > 0 && ownColor == PlayerColor::BLACK)) 
	{
		string errorMessage = "PieceList:movePiece: trying to put piece of wrong color in PieceList";
		cout << errorMessage << endl;
		throw std::invalid_argument(errorMessage);
	}
	
	add(move.piece, move.to);
	removePiece(move.from);
}
void PieceList::removePiece(Square square) 
{
	pieces.erase(SquareToIndex(square));
}
void PieceList::clearList() 
{
	pieces.clear();
}
const_iterator_t PieceList::begin() const
{
	return pieces.begin();
}
const_iterator_t PieceList::end() const
{
	return pieces.end();
}
int PieceList::getPieceType(Square sq) 
{
	
	return pieces[SquareToIndex(sq)].type;
}
