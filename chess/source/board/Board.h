// --------------------------------------------------------------------------------
//									 Board
// --------------------------------------------------------------------------------

#ifndef _BOARD_H
#define _BOARD_H

// ------ Includes -----


#include <iostream>
#include <stdio.h>
#include <vector>
#include <array>
#include <set>
#include "source/SharedTypes.h"
#include "source/board/piece.h"

using namespace std;

class Board
{
public:

	Board();
	~Board();

	int getPieceOnSquare(Square sq) const;
	int getPieceOnSquare(int sqY, int sqX) const;
	bool IsEmptySquare(Square sq) const;

	int getPieceOnWhiteOutedSquare(int square) const;
	int getPieceOnBlackOutedSquare(int square) const;

	Square GetKingPos(PlayerColor pl) const;

	PlayerColor getTurn() const;
	void setTurn(PlayerColor turn);

	bool getCastelingPossible(casteling castelingType) const;
	void setCastelingRight(casteling castelingType, bool possible);

	int POV(int rankNumber) const;
	vector<int> outedWhite;
	vector<int> outedBlack;

	void SetKingPos(PlayerColor color, Square sq);
	void SetPieceOnSquare(int piece, Square sq);
	void SetPieceOnSquare(int piece, int rank, int file);

protected:

private:

	Square whiteKing;
	Square blackKing;

	array<array<int, 8>, 8> board;  // matrix representation of pieces on the board

	PlayerColor turn;

	BoolStruct castelingPossible;
};

#endif // _BOARD_
