// --------------------------------------------------------------------------------
//									 Board
// --------------------------------------------------------------------------------

#ifndef _BOARD_
#define _BOARD_

// ------ Includes -----


#include <iostream>
#include <stdio.h>
#include <vector>
#include <set>
#include "source/SharedConstants.h"

using namespace std;

class Board
{
public:

	Board						();
	~Board();

	int getPieceOnSquare		(Square sq);
	int getPieceOnSquare(int sqY, int sqX);
	int getPieceOnSquare(Player player, int index);
	static void IndexToSquare(int index, Square* square);

	bool IsEmptySquare			(Square sq);
	void makeMoveFromTo			(Square from, Square to);
	void PrintInt				(int var, std::string varName);
	void InitBoard				(int initMatrix[8][8]);
	int getPieceOnWhiteOutedSquare(int square);
	int getPieceOnBlackOutedSquare(int square);

	set<int> getWhiteAlivePieceSet();
	set<int> getBlackAlivePieceSet();

	Square GetKingPos(Player pl);
	
	bool IsFriendlyPiece(int piece);
	bool IsFriendlyPiece(Square sq);

	int POV(int rankNumber);
	bool getCastelingPossible(casteling castelingType);

private:

	Square whiteKing;
	Square blackKing;
	vector<int> outedWhite;
	vector<int> outedBlack;

	int board[8][8];  // matrix representation of pieces on the board

	set<int> whiteAlivePieceIdxs; // second representation of pieces,
	set<int> blackAlivePieceIdxs; //  sometimes it is inconvinent to loop to all squares in board
	// 64 squares, index row by row from top left
	// index 0 is x=0, y=0, index 7 is x7, y=0, index 63 is x=7, y=7
	int SquareToIndex(Square square);
	int SquareToIndex(int sqY, int sqX);

	BoolStruct castelingPossible;
	void SetKingPos(Square sq);
	void SetPieceOnSquare(int piece, Square sq);
	void SetPieceOnSquare(int piece, int y, int x);
	void MovePiece(Square from, Square to);
	void MovePieceKill(Square from, Square to);
	void RemovePiece(Square toSq);
	void PromoteQueen(Square square);
	
};

#endif // _BOARD_
