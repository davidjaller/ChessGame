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

	const_iterator_t piecesBegin(PlayerColor color) const;
	const_iterator_t piecesEnd(PlayerColor color) const;

	bitBoard_t getOccupiedSquaresBB(PlayerColor color) const;
	bitBoard_t getAttackedSquaresBB(PlayerColor attackingColor) const;

	void MovePiece(Square from, Square to);
	void RemovePiece(Square toSq);
	PlayerColor getTurn() const;

	static bool SquareIsAttacked(const Position* position, Square square, vector<Square>* attackingSquares,
		PlayerColor attackingColor);
	static bool SquareIsAttacked(const Position* position, Square square, PlayerColor attackingColor);
	static bool KingIsChecked(const Position* position, vector<Square>* attackingSquares, PlayerColor kingColor);

private:
	Board board;
	PieceList whiteAlivePieces;
	PieceList blackAlivePieces;
	// 64 squares, index row by row from top left
	// index 0 is file=0, rank=0, index 7 is x7, rank=0, index 63 is file=7, rank=7
};

#endif // !_fundamental_board_h
