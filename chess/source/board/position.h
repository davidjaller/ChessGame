#ifndef _position_h
#define _position_h
#include "source/board/board.h"
#include "source/board/PieceList.h"
#include <map>
#include<sstream>

using namespace std;
using pieces_t = map<int, Piece>;
using const_iterator_t = pieces_t::const_iterator;

class Position {
public:
	Position();

	void InitPosition(array<array<int, 8>, 8> initMatrix, PlayerColor turn = PlayerColor::WHITE);
	void InitPosition(const string &fenStr);

	

	void makeMoveFromTo(Square from, Square to);
	bool IsEmptySquare(Square sq) const;
	bool IsFriendlyPiece(int piece) const;
	bool IsFriendlyPiece(Square sq) const;
	bool IsEnemyPiece(Square sq) const;
	void PromoteQueen(Square square);

	Square getKingPos(PlayerColor kingColor) const;
	bool getCastelingPossible(casteling castelingType) const;
	const Piece* getPieceOnSquare(Square square, PlayerColor color) const;

	const_iterator_t piecesBegin(PlayerColor color) const;
	const_iterator_t piecesEnd(PlayerColor color) const;

	bitBoard_t getOccupiedSquaresBB(PlayerColor color) const;
	bitBoard_t getAttackedSquaresBB(PlayerColor attackingColor) const;


	PlayerColor getTurn() const;
	void setTurn(PlayerColor turn);

	bool PieceIsAttacked(Square square, list<const Piece*>* attackingPieces,
		PlayerColor attackingColor) const;
	bool PieceIsAttacked(Square square, PlayerColor attackingColor)const;
	bool KingIsChecked(list<const Piece*>* attackingPieces, PlayerColor kingColor) const;
	void updateAllPieces();
	void updateAffectedPieces(Move move, const Board* board);

	const Board* getBoard() const;

private:
	Board board;
	PieceList whiteAlivePieces;
	PieceList blackAlivePieces;
	void setPiece(int pieceType, Square sq);
	void MovePiece(Move move);
	void RemovePiece(Square toSq);
};

#endif 
