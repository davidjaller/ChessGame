
#include "source/board/Position.h"

Position::Position() : whiteAlivePieces(PlayerColor::WHITE), blackAlivePieces(PlayerColor::BLACK) 
{

}

// This is usefull for unit testing
void Position::InitPosition(array<array<int, 8>, 8> initMatrix, PlayerColor turn)
{
	blackAlivePieces.clearList();
	whiteAlivePieces.clearList();
	Square sq;
	for (sq.rank = 0; sq.rank < 8; sq.rank++)
	{
		for (sq.file = 0; sq.file < 8; sq.file++)
		{
			int pieceType = initMatrix[sq.rank][sq.file];
			board.SetPieceOnSquare(pieceType, sq);

			if (pieceType > 0) {
				whiteAlivePieces.add(pieceType, sq);
				if (pieceType == WHITE_KING) {
					board.SetKingPos(PlayerColor::WHITE, sq);
				}
			}
			else if (pieceType < 0) {
				blackAlivePieces.add(pieceType, sq);
				if (pieceType == BLACK_KING) {
					board.SetKingPos(PlayerColor::BLACK, sq);
				}
			}
		}
	}
	board.setCastelingRight(WHITE_SHORT, true); // for now constant, could be argument
	board.setCastelingRight(WHITE_LONG, true);
	board.setCastelingRight(BLACK_SHORT, true);
	board.setCastelingRight(BLACK_LONG, true);

	board.outedWhite.clear();
	board.outedBlack.clear();
	board.setTurn(turn);
}

const Board* Position::getBoard() const
{
	return &board;
}

PlayerColor Position::getTurn() const
{
	return board.getTurn();
}

void Position::setTurn(PlayerColor turn) 
{
	board.setTurn(turn);
}

Square Position::getKingPos(PlayerColor kingColor) const
{
	return board.GetKingPos(kingColor);
}

bool Position::getCastelingPossible(casteling castelingType) const 
{
	return board.getCastelingPossible(castelingType);
}

bitBoard_t Position::getOccupiedSquaresBB(PlayerColor color) const
{
	if (color == PlayerColor::WHITE) 
		return whiteAlivePieces.getOccupiesBB();
	else
		return blackAlivePieces.getOccupiesBB();
}

bitBoard_t Position::getAttackedSquaresBB(PlayerColor attackingColor) const
{
	if (attackingColor == PlayerColor::WHITE) 
		return whiteAlivePieces.getAttackingBB();
	else
		return blackAlivePieces.getAttackingBB();
}

bool Position::IsEmptySquare(Square sq) const
{
	if (board.getPieceOnSquare(sq) == 0)
		return true;
	else
		return false;
}

bool Position::IsFriendlyPiece(int piece) const
{
	if ((getTurn() == PlayerColor::WHITE && piece > 0)
		|| (getTurn() == PlayerColor::BLACK && piece < 0))
		return true;
	else
		return false;
}

bool Position::IsEnemyPiece(Square sq) const
{
	int piece = board.getPieceOnSquare(sq);
	PlayerColor turn = getTurn();
	if (turn == PlayerColor::WHITE &&  piece < 0
		|| turn == PlayerColor::BLACK && piece > 0)
		return true;
	else
		return false;
}

bool Position::IsFriendlyPiece(Square square) const
{
	int piece = board.getPieceOnSquare(square);
	return IsFriendlyPiece(piece);
}

//void Position::MovePieceKill(Square from, Square to) {
//
//	RemovePiece(to);
//	MovePiece(from, to);
//}
//
//void Position::MovePieceNoKill(Square from, Square to) {
//	MovePiece(from, to);
//}

void Position::MovePiece(Move move) {
	if(move.kill)
		RemovePiece(move.to);

	board.SetPieceOnSquare(move.piece, move.to);
	board.SetPieceOnSquare(0, move.from);

	if (getTurn() == PlayerColor::WHITE) 
		whiteAlivePieces.movePiece(move);
	else
		blackAlivePieces.movePiece(move);

	whiteAlivePieces.updateMovement(move, &board, getTurn());
	blackAlivePieces.updateMovement(move, &board, getTurn());
}

void Position::RemovePiece(Square sq) {

	int piece = board.getPieceOnSquare(sq);

	if (piece>0) {
		whiteAlivePieces.removePiece(sq);
		board.outedWhite.push_back(piece);
	}
	else if(piece < 0){
		blackAlivePieces.removePiece(sq);
		board.outedBlack.push_back(piece);
	}
}

void Position::PromoteQueen(Square square) {

	// Move into pieceList Move piece and with promote queen as argument argument
	// We need to update the movement acoarding to this

	int piece = board.getPieceOnSquare(square);
	RemovePiece(square);

	if (getTurn() == PlayerColor::WHITE) 
	{
		board.SetPieceOnSquare(WHITE_QUEEN, square);
		whiteAlivePieces.removePiece(square);
		whiteAlivePieces.add(QUEEN, square);
	}
	else
	{
		board.SetPieceOnSquare(BLACK_QUEEN, square);
		blackAlivePieces.removePiece(square);
		blackAlivePieces.add(QUEEN, square);
	}
}

const_iterator_t Position::piecesBegin(PlayerColor color) const
{
	if (color == PlayerColor::WHITE)
		return whiteAlivePieces.begin();
	else
		return blackAlivePieces.begin();
}

const_iterator_t Position::piecesEnd(PlayerColor color) const
{
	if (color == PlayerColor::WHITE)
		return whiteAlivePieces.end();
	else
		return blackAlivePieces.end();
}

void Position::makeMoveFromTo(Square from, Square to)
{
	int fromPiece = board.getPieceOnSquare(from);
	int toPiece = board.getPieceOnSquare(to);

	Move move;
	move.from = from;
	move.to = to;
	move.piece = fromPiece;

	PlayerColor turn = getTurn();
	
	// If rook moves update casteling possablity
	if (fromPiece == WHITE_ROOK && from.rank == 0)
		board.setCastelingRight(WHITE_SHORT, false);

	else if (fromPiece == WHITE_ROOK && from.file == 7)
		board.setCastelingRight(WHITE_LONG, false);

	else if (fromPiece == BLACK_ROOK && from.file == 0)
		board.setCastelingRight(BLACK_SHORT, false);

	else if (fromPiece == BLACK_ROOK && from.file == 7)
		board.setCastelingRight(BLACK_LONG, false);

	// king move
	else if (abs(fromPiece) == KING)
	{
		// Check if a casteling move and move the rook
		int firstRank = getFirstRank(turn);
		if (from.file == 4 && from.rank == firstRank && to.rank == firstRank) {
			Move rookMove;
			rookMove.from = from;
			rookMove.piece = ROOK *(1- 2*(int)turn);
			rookMove.kill = false;
			rookMove.to = to;
			if (to.file == 2) { // long
				rookMove.from.file = 0;
				rookMove.to.file = 3;
				MovePiece(rookMove);
			}
			else if (to.file == 6) { // short
				rookMove.from.file = 7;
				rookMove.to.file = 5;
				MovePiece(rookMove);
			}
		}

		// if king move then casteling not possible in future
		if (turn == PlayerColor::WHITE)
		{
			board.setCastelingRight(WHITE_SHORT, false);
			board.setCastelingRight(WHITE_LONG, false);
		}
		else if (turn == PlayerColor::BLACK)
		{
			board.setCastelingRight(BLACK_SHORT, false);
			board.setCastelingRight(BLACK_LONG, false);
		}
		board.SetKingPos(turn, to);
	}
	if (!IsEmptySquare(to))
		move.kill = true;
	else
		move.kill = false;
	
	//pawn promotion
	if (abs(fromPiece) == PAWN && to.rank == getLastRank(turn)) {
		//move.piece = QUEEN;
		PromoteQueen(to);
	}
	MovePiece(move);
}

void Position::updateAllPieces() 
{
	whiteAlivePieces.updateAllMovements(&board);
	blackAlivePieces.updateAllMovements(&board);

}

void Position::updateAffectedPieces(Move move, const Board* board)
{
	whiteAlivePieces.updateMovement(move, board, getTurn());
	blackAlivePieces.updateMovement(move, board, getTurn());

}

bool Position::KingIsChecked(list<const Piece*>* attackingSquares, PlayerColor kingColor) const {
	return PieceIsAttacked(getKingPos(kingColor), attackingSquares, getOpposite(kingColor));
}

const Piece* Position::getPieceOnSquare(Square square, PlayerColor color) const {
	if (color == PlayerColor::WHITE) 
		return whiteAlivePieces.getPieceOnSquare(square);
	else
		return blackAlivePieces.getPieceOnSquare(square);
}

bool Position::PieceIsAttacked(Square pieceSquare, list<const Piece*>* attackingPieces, PlayerColor attackingColor) const
{
	if ((getAttackedSquaresBB(attackingColor) & squareToBitBoard(pieceSquare)) > 0)
	{
		if (attackingColor == PlayerColor::WHITE)
			whiteAlivePieces.getPiecesAttackingPiece(pieceSquare, attackingPieces);
		else
			blackAlivePieces.getPiecesAttackingPiece(pieceSquare, attackingPieces);
		return true;
	}
	else
		return false;
}

// Overloaded method without output pointer 
bool Position::PieceIsAttacked(Square pieceSquare, PlayerColor attackingColor) const
{
	return (getAttackedSquaresBB(attackingColor) & squareToBitBoard(pieceSquare)) > 0;
}
