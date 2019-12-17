/*****************************************************************************************
/* File: Board.cpp

/*
/*****************************************************************************************/

// ----- Includes -----

#include "source/board/Board.h"
#include "source/Game.h"
#include "windows.h"
#include <stdio.h>
#include <math.h>
#include <string>
using namespace std;

array<array<int, 8>, 8> INIT_BOARD = { {
						{-4, -3, -2, -5, 0, -2, -3, -4},
						{-1, -1, -1, -1, -6, -1, -1, -1},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 1,  1,  1,  1,  5,  1,  1,  1},
						{ 4,  3,  2,  1,  6,  0,  0,  4}
} };
//array<array<int, 8>, 8> INIT_BOARD = { {
//						{ 0,  0,  0,  0,  0,  0,  0,  0},
//						{ 0,  0,  0,  0,  0,  0,  0,  0},
//						{ 0,  0,  0,  0,  0,  0,  0,  0},
//						{ -1,  0,  0,  0,  0,  0,  0, 0},
//						{ 0,   1,  0,  0,  0,  0,  0, 0},
//						{ 0,  0,  0,  0,  0,  0,  0,  0},
//						{ 0,  0,  0,  0,  0,  0,  0,  0},
//						{ 0,  0,  0,  0,  0,  0,  0,  0}
//} };



Board::Board()
{
	board = { {} };

}

Board::~Board() {}

Square Board::GetKingPos(PlayerColor pl) const
{

	if (pl == PlayerColor::WHITE)
		return whiteKing;
	else
		return blackKing;
}

PlayerColor Board::getTurn() const {
	return turn;
}
void Board::setTurn(PlayerColor turn) {
	this->turn = turn;
}

void Board::SetKingPos(PlayerColor color, Square sq)
{
	if (color == PlayerColor::WHITE)
	{
		whiteKing = sq;
	}
	else
	{
		blackKing = sq;
	}
}

int Board::POV(int rankNumber) const {
	if (turn == PlayerColor::BLACK)
		return rankNumber;
	else
		return 7 - rankNumber;
}

bool Board::IsEmptySquare(Square sq) const
{
	if (getPieceOnSquare(sq) == 0)
		return true;
	else
		return false;
}


int Board::getPieceOnSquare(Square sq) const
{
	if (sq.file > 7 || sq.rank > 7 || sq.file < 0 || sq.rank < 0) {
		(void)fprintf(stderr, "iligal square : \n");
		return 0;
	}
	return board[sq.rank][sq.file];
}

int Board::getPieceOnSquare(int sqY, int sqX) const {
	return board[sqY][sqX];
}

void Board::SetPieceOnSquare(int piece, Square sq)
{
	if (sq.rank >= 0 && sq.rank < 8 && sq.file >= 0 && sq.file < 8) {
		board[sq.rank][sq.file] = piece;
	}
	else
		cout << "SetPieceOnSquare(): Out of bounds" << endl;
}

void Board::SetPieceOnSquare(int piece, int sqY, int sqX)
{
	board[sqY][sqX] = piece;
}

int Board::getPieceOnWhiteOutedSquare(int squareNr) const
{
	if (squareNr < outedWhite.size())
		return outedWhite[squareNr];
	else
		return 0;
}

int Board::getPieceOnBlackOutedSquare(int square) const
{
	if (square < outedBlack.size())
		return outedBlack[square];
	else
		return 0;
}


bool Board::getCastelingPossible(casteling castelingType) const {
	switch (castelingType) {
	case BLACK_LONG:
		return castelingPossible.blackLong;
	case BLACK_SHORT:
		return castelingPossible.blackShort;
	case WHITE_SHORT:
		return castelingPossible.whiteShort;
	case WHITE_LONG:
		return castelingPossible.whiteLong;
	default:
		return false;
	}
}

void Board::setCastelingRight(casteling castelingType, bool possible) {
	switch (castelingType) {
	case BLACK_LONG:
		 castelingPossible.blackLong = possible;
		 break;
	case BLACK_SHORT:
		castelingPossible.blackShort = possible;
		break;
	case WHITE_SHORT:
		castelingPossible.whiteShort = possible;
		break;
	case WHITE_LONG:
		castelingPossible.whiteLong = possible;
		break;
	default:
		printf("setCastelingNotPossible: illegal argument");
		break;
	}
}

