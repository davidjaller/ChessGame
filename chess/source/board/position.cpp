
#include "source/board/Position.h"

Position::Position() {};

// This is usefull for unit testing
void Position::InitPosition(array<array<int, 8>, 8> initMatrix, PlayerColor turn)
{
	Square sq;
	for (int sq.rank = 0; sq.rank < 8; sq.rank++)
	{
		for (int sq.file = 0; sq.file < 8; sq.file++)
		{
			pieceType = initMatrix[sq.rank][sq.file];
			board.setPieceOnSquare(sq) = pieceType;
			if (board[rank][file] > 0) {
				Piece piece = Piece(abs(pieceType, sq, PlayerColor::WHITE));
				whiteAlivePieces.add(piece, sq));
				if (pieceType == WHITE_KING) {
					setWhiteKing(sq);
				}
			}
			else if (board[rank][file] < 0) {
				Piece piece = Piece(abs(pieceType, sq, PlayerColor::BLACK));
				blackAlivePieces.add(piece, sq);
				if (pieceType== BLACK_KING) {
					setBlackKing(sq);
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

const Board* Position::getBoard() {
	return board;
}

PlayerColor Position::getTurn() const{
	return board.getTurn();
}

Square Position::getKingPos(PlayerColor kingColor) const{
	return boad.getKingPos(kingColor);
}

bool Position::getCastelingPossible(casteling castelingType) const {
	return boar.getCastelingPosible(castelingType);
}

bitBoard_t Position::getOccupiedSquaresBB(PlayerColor color) const{
	if (color == PlayerColor::WHITE) {
		return whiteAlivePieces.getOccupiesBB();
	else
		return blackAlivePieces.getOccupiesBB();
	}

}
bitBoard_t Position::getAttackedSquaresBB(PlayerColor attackingColor) const{
	if (attackingColor == PlayerColor::WHITE) {
		return whiteAlivePieces.getAttackingBB();
	else
		return blackAlivePieces.getAttackingBB();
	}
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
		whiteAlivePieces.movePiece(from, to);
	}
	else if (fromPiece < 0) { // black
		blackAlivePieceIdxs.movePiece(from, to);
	}
}

void Position::RemovePiece(Square sq) {

	int piece = board.getPieceOnSquare(sq);

	if (piece > 0) {
		whiteAlivePieces.remove(sq);
		board.outedWhite.push_back(piece);
	}
	else if (piece < 0) {
		blackAlivePieces.remove(sq);
		board.outedBlack.push_back(piece);
	}
}

void Position::PromoteQueen(Square square) {

	int piece = board.getPieceOnSquare(square);
	RemovePiece(square);

	if (piece > 0) {
		SetPieceOnSquare(WHITE_QUEEN, square);
		whiteAlivePieces.remove(square);
		whiteAlivePieces.add(WHITE_QUEEN, square)
	}
	else if (piece < 0)
		SetPieceOnSquare(BLACK_QUEEN, square);
		blackAlivePieces.remove(square);
		blackAlivePieces.add(BLACK_QUEEN, square)
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



void Position::makeMoveFromTo(Square from, Square to)
{
	int fromPiece = board.getPieceOnSquare(from);
	int toPiece = board.getPieceOnSquare(to);

	// If rook moves update casteling posablity
	if (fromPiece == WHITE_ROOK && from.rank == 0)
		setCastelingRight(WHITE_SHORT, false);

	else if (fromPiece == WHITE_ROOK && from.file == 7)
		setCastelingRight(WHITE_LONG, false);

	else if (fromPiece == BLACK_ROOK && from.file == 0)
		setCastelingRight(BLACK_SHORT, false);

	else if (fromPiece == BLACK_ROOK && from.file == 7)
		setCastelingRight(BLACK_LONG, false);

	// king move
	else if (abs(fromPiece) == KING)
	{
		// Check if a casteling move and move the rook
		if (from.file == 4 && POV(from.rank) == 0 && POV(to.rank) == 0) {
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
		if (getTurn() == PlayerColor::WHITE)
		{
			setCastelingRight(WHITE_SHORT, false);
			setCastelingRight(WHITE_LONG, false);
		}
		else if (getTurn() == PlayerColor::BLACK)
		{
			setCastelingRight(BLACK_SHORT, false);
			setCastelingRight(BLACK_LONG, false);
		}
		SetKingPos(to);
	}
	if (!IsEmptySquare(to))
		MovePieceKill(from, to);
	else
		MovePiece(from, to);

	//pawn promotion
	if (abs(fromPiece) == PAWN && (to.rank == 0 || to.rank == 7))
	{
		PromoteQueen(to);
	}
}

bool Position::KingIsChecked(vector<Square>* attackingSquares, PlayerColor kingColor) const {
	return SquareIsAttacked(getKingPos(kingColor), attackingSquares, getOpposite(kingColor));
}

Piece* Position::getPieceOnSquare(Square square, PlayerColor color) const {
	if (color == PlayerColor::WHITE) 
		return whiteAlivePieces.getPieceOnSquare(square);
	else
		return blackAlivePieces.getPieceOnSquare(square);
}

bool Position::SquareIsAttacked(Square square, vector<Square>* attackingSquares, PlayerColor attackingColor)
{

	attackingSquares->clear();
	Square sq;

	bitBoard_t attackersBB = getAttackedSquaresBB(attackingColor) & squareToBitBoard(square);
	if (attackersBB != 0) {
		bitBoardToSquares(attackingSquares, attackersBB);
		return true;
	}
	else
		return false;

}

// Overloaded method without output pointer
bool Position::SquareIsAttacked( Square square, PlayerColor attackingColor) {

	return position->getAttackedSquaresBB(attackingColor) & squareToBitBoard(square);
}