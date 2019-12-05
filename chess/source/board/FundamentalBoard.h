// --------------------------------------------------------------------------------
//									 FundamentalBoard
// --------------------------------------------------------------------------------

#ifndef FUNDAMENTAL_BOARD_H
#define FUNDAMENTAL_BOARD_H

// ------ Includes -----


#include <iostream>
#include <stdio.h>
#include <vector>
#include <array>
#include <set>
#include "source/SharedTypes.h"

using namespace std;

class FundamentalBoard
{
public:

	FundamentalBoard();
	~FundamentalBoard();
	void InitBoard(array<array<int, 8>, 8> initMatrix, PlayerColor turn = PlayerColor::WHITE);

	int getPieceOnSquare(Square sq) const;
	int getPieceOnSquare(int sqY, int sqX) const;
	int getPieceOnSquare(PlayerColor player, int index) const;

	int getPieceOnWhiteOutedSquare(int square) const;
	int getPieceOnBlackOutedSquare(int square) const;

	set<int> getWhiteAlivePieceSet() const;
	set<int> getBlackAlivePieceSet() const;
	set<int> getAlivePieceSet(PlayerColor color) const;

	Square GetKingPos(PlayerColor pl) const;

	PlayerColor getTurn() const;
	void setTurn(PlayerColor turn);

	bool getCastelingPossible(casteling castelingType) const;
	void setCastelingNotPossible(casteling castelingType);

	int POV(int rankNumber) const;
	static int SquareToIndex(Square square);
	static int SquareToIndex(int sqY, int sqX);
	static void IndexToSquare(int index, Square* square);

protected:

	void SetKingPos(Square sq);
	void SetPieceOnSquare(int piece, Square sq);
	void SetPieceOnSquare(int piece, int rank, int file);
	void MovePiece(Square from, Square to);
	void RemovePiece(Square toSq);


private:

	Square whiteKing;
	Square blackKing;
	vector<int> outedWhite;
	vector<int> outedBlack;

	array<array<int, 8>, 8> board;  // matrix representation of pieces on the board

	PlayerColor turn;

	set<int> whiteAlivePieceIdxs; // second representation of pieces,
	set<int> blackAlivePieceIdxs; //  sometimes it is inconvinent to loop to all squares in board
	// 64 squares, index row by row from top left
	// index 0 is file=0, rank=0, index 7 is x7, rank=0, index 63 is file=7, rank=7

	BoolStruct castelingPossible;


};

#endif // _BOARD_
