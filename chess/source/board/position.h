#ifndef _position_h
#define _position_h
#include "source/board/board.h"
#include <map>
using namespace std;
using pieces_t = map<int, Piece>;
using const_iterator_t = pieces_t::const_iterator;

class Position {
public:
	Position();

	void InitPosition(array<array<int, 8>, 8> initMatrix, PlayerColor turn = PlayerColor::WHITE);

	void makeMoveFromTo(Square from, Square to);
	bool IsEmptySquare(Square sq) const;
	bool IsFriendlyPiece(int piece) const;
	bool IsFriendlyPiece(Square sq) const;
	bool IsEnemyPiece(Square sq) const;
	void PromoteQueen(Square square);
	void MovePieceKill(Square from, Square to);
	void MovePieceNoKill(Square from, Square to);

	Square getKingPos(PlayerColor kingColor) const;
	bool getCastelingPossible(casteling castelingType) const;
	Piece* getPieceOnSquare(Square square, PlayerColor color) const;

	const_iterator_t piecesBegin(PlayerColor color) const;
	const_iterator_t piecesEnd(PlayerColor color) const;

	bitBoard_t getOccupiedSquaresBB(PlayerColor color) const;
	bitBoard_t getAttackedSquaresBB(PlayerColor attackingColor) const;

	void MovePiece(Square from, Square to);
	void RemovePiece(Square toSq);
	PlayerColor getTurn() const;

	bool SquareIsAttacked(Square square, vector<Square>* attackingSquares,
		PlayerColor attackingColor);
	bool SquareIsAttacked(Square square, PlayerColor attackingColor);
	bool KingIsChecked(vector<Square>* attackingSquares, PlayerColor kingColor) const;

	const Board* getBoard();

private:
	Board board;
	PieceList whiteAlivePieces;
	PieceList blackAlivePieces;

};

#endif 
