#ifndef _board_h
#define _board_h
#include "source/board/FundamentalBoard.h"
class FundamentalBoard; // forward declaration

class Board : public FundamentalBoard {
public:
	Board();
	void makeMoveFromTo(Square from, Square to);
	bool IsEmptySquare(Square sq) const;
	bool IsFriendlyPiece(int piece) const;
	bool IsFriendlyPiece(Square sq) const;
	void PromoteQueen(Square square);
	void MovePieceKill(Square from, Square to);
	void MovePieceNoKill(Square from, Square to);
};

#endif // !_fundamental_board_h
