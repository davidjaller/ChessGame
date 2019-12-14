
#include "source/board/Position.h"

Position::Position() : whiteAlivePieces(PlayerColor::WHITE), blackAlivePieces(PlayerColor::BLACK) {};

// This is usefull for unit testing
void Position::InitPosition(array<array<int, 8>, 8> initMatrix, PlayerColor turn)
{
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
	board.setCastelingRight(WHITE_SHORT, true); // for now constant, should be argument
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

void Position::MovePieceKill(Square from, Square to) {

	RemovePiece(to);
	MovePiece(from, to);
}

void Position::MovePieceNoKill(Square from, Square to) {
	MovePiece(from, to);
}

void Position::MovePiece(Square from, Square to) {

	int fromPiece = board.getPieceOnSquare(from);
	board.SetPieceOnSquare(fromPiece, to);
	board.SetPieceOnSquare(0, from);

	if (fromPiece > 0) { // white
		whiteAlivePieces.movePiece(fromPiece, from, to);
	}
	else if (fromPiece < 0) { // black
		blackAlivePieces.movePiece(fromPiece, from, to);
	}
}

void Position::RemovePiece(Square sq) {

	int piece = board.getPieceOnSquare(sq);

	if (piece > 0) {
		whiteAlivePieces.removePiece(sq);
		board.outedWhite.push_back(piece);
	}
	else if (piece < 0) {
		blackAlivePieces.removePiece(sq);
		board.outedBlack.push_back(piece);
	}
}

void Position::PromoteQueen(Square square) {

	int piece = board.getPieceOnSquare(square);
	RemovePiece(square);

	if (piece > 0) {
		board.SetPieceOnSquare(WHITE_QUEEN, square);
		whiteAlivePieces.removePiece(square);
		whiteAlivePieces.add(WHITE_QUEEN, square);
	}
	else if (piece < 0)
		board.SetPieceOnSquare(BLACK_QUEEN, square);
		blackAlivePieces.removePiece(square);
		blackAlivePieces.add(BLACK_QUEEN, square);
}

const_iterator_t Position::piecesBegin(PlayerColor color) const{
	if (color == PlayerColor::WHITE)
		return whiteAlivePieces.begin();
	else
		return blackAlivePieces.begin();
}

const_iterator_t Position::piecesEnd(PlayerColor color) const{
	if (color == PlayerColor::WHITE)
		return whiteAlivePieces.end();
	else
		return blackAlivePieces.end();
}

bool Position::IsLegalMove(Square squareFrom, Square squareTo) {
	const Piece* piece;
	PieceList* pieces;
	if (getTurn() == PlayerColor::WHITE)
		pieces = &whiteAlivePieces;
	else
		pieces = &blackAlivePieces;

	if (!pieces->getPieceOnSquare(squareFrom, piece))
		return false;
	else {
		if ((piece->canKillOnBB | piece->canMoveToBB) & squareToBitBoard(squareTo))
			return true;
		else
			return false;
	}
}

void Position::makeMoveFromTo(Square from, Square to)
{
	int fromPiece = board.getPieceOnSquare(from);
	int toPiece = board.getPieceOnSquare(to);

	PlayerColor turn = getTurn();
	
	// If rook moves update casteling posablity
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
			Square rookFrom = from;
			Square rookTo = to;
			if (to.file == 2) { // long
				rookFrom.file = 0;
				rookTo.file = 3;
				MovePiece(rookFrom, rookTo);
			}
			else if (to.file == 6) { // short
				rookFrom.file = 7;
				rookTo.file = 5;
				MovePiece(rookFrom, rookTo);
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
		MovePieceKill(from, to);
	else
		MovePiece(from, to);

	//pawn promotion
	if (abs(fromPiece) == PAWN && to.rank == getLastRank(turn))
		PromoteQueen(to);
	
}

bool Position::KingIsChecked(list<const Piece*>* attackingSquares, PlayerColor kingColor) const {
	return PieceIsAttacked(getKingPos(kingColor), attackingSquares, getOpposite(kingColor));
}

void Position::getPieceOnSquare(Square square, PlayerColor color, const Piece* piece) const {
	if (color == PlayerColor::WHITE) 
		whiteAlivePieces.getPieceOnSquare(square, piece);
	else
		blackAlivePieces.getPieceOnSquare(square, piece);
}

bool Position::PieceIsAttacked(Square pieceSquare, list<const Piece*>* attackingPieces, PlayerColor attackingColor) const{

	Square sq;

	bitBoard_t attackersBB = getAttackedSquaresBB(attackingColor) & squareToBitBoard(pieceSquare);
	if (attackersBB != 0) {

		if (attackingColor == PlayerColor::WHITE)
			whiteAlivePieces.getPiecesAttackingPiece(pieceSquare, attackingPieces);
		else
			blackAlivePieces.getPiecesAttackingPiece(pieceSquare, attackingPieces);
		
		return true;
	}
	else
		return false;

}

// Overloaded method without output pointer output
bool Position::PieceIsAttacked(Square pieceSquare, PlayerColor attackingColor) const{

	return (getAttackedSquaresBB(attackingColor) & squareToBitBoard(pieceSquare)) != 0;
}