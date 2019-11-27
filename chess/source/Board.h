// --------------------------------------------------------------------------------
//									 Board
// --------------------------------------------------------------------------------

#ifndef _BOARD_
#define _BOARD_

// ------ Includes -----


#include <iostream>
#include <stdio.h>
#include "source/SharedConstants.h"

class Board
{
public:

	Board						();
	
	int outedCountWhite;
	int outedCountBlack;
	
	int getPieceOnSquare		(Square sq);

	void SetPieceOnSquare		(int piece, Square sq);
	bool IsEmptySquare			(Square sq);
	void makeMoveFromTo			(Square from, Square to);
	void PrintInt				(int var, std::string varName);
	void InitBoard				();
	int getPieceOnWhiteOutedSquare(int square);
	int getPieceOnBlackOutedSquare(int square);

	Square GetKingPos(Player pl);
	
	bool IsFriendlyPiece(int piece);
	bool IsFriendlyPiece(Square sq);
	void SetKingPos(Square sq);
	int POV(int rankNumber);
	bool getCastelingPossible(casteling castelingType);

private:

	int board[8][8];
	Square whiteKing;
	Square blackKing;
	int outedWhite[16];
	int outedBlack[16];

	BoolStruct castelingPossible;
};

#endif // _BOARD_
