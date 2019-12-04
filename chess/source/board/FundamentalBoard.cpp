/*****************************************************************************************
/* File: FundamentalBoard.cpp

/*
/*****************************************************************************************/

// ----- Includes -----

#include "source/board/FundamentalBoard.h"
#include "source/Game.h"
#include "windows.h"
#include <stdio.h>
#include <math.h>
#include <string>
using namespace std;

array<array<int, 8>, 8> INIT_BOARD = { {
						{-4, -3, -2, -5, -6, -2, -3, -4},
						{-1, -1, -1, -1, -1, -1, -1, -1},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 1,  1,  1,  1,  0,  1,  1,  1},
						{ 4,  3,  2,  5,  6,  0,  0,  4}
} };



FundamentalBoard::FundamentalBoard()
{
	InitBoard(INIT_BOARD);
}

FundamentalBoard::~FundamentalBoard() {}

// This is usefull for unit testing
void FundamentalBoard::InitBoard(array<array<int, 8>, 8> initMatrix, PlayerColor turn)
{
	for (int y = 0; y < 8; y++)
	{
		for (int x = 0; x < 8; x++)
		{
			board[y][x] = initMatrix[y][x];
			if (board[y][x] > 0) {
				whiteAlivePieceIdxs.insert(SquareToIndex(y, x));
				if (board[y][x] == WHITE_KING) {
					whiteKing.x = x;
					whiteKing.y = y;
				}
			}
			else if (board[y][x] < 0) {
				blackAlivePieceIdxs.insert(SquareToIndex(y, x));
				if (board[y][x] == BLACK_KING) {
					blackKing.x = x;
					blackKing.y = y;
				}
			}
		}
	}
	castelingPossible.blackLong = 1;
	castelingPossible.blackShort = 1;
	castelingPossible.whiteLong = 1;
	castelingPossible.whiteShort = 1;
	this->turn = turn;
}

Square FundamentalBoard::GetKingPos(PlayerColor pl) const
{

	if (pl == PlayerColor::WHITE)
		return whiteKing;
	else
		return blackKing;
}

PlayerColor FundamentalBoard::getTurn() const {
	return turn;
}
void FundamentalBoard::setTurn(PlayerColor turn) {
	this->turn = turn;
}

void FundamentalBoard::SetKingPos(Square sq)
{
	if (turn == PlayerColor::WHITE)
	{
		whiteKing = sq;
	}
	else
	{
		blackKing = sq;
	}
}

int FundamentalBoard::POV(int rankNumber) const {
	if (turn == PlayerColor::BLACK)
		return rankNumber;
	else
		return 7 - rankNumber;
}


int FundamentalBoard::getPieceOnSquare(Square sq) const
{
	if (sq.x > 7 || sq.y > 7 || sq.x < 0 || sq.y < 0) {
		(void)fprintf(stderr, "iligal square : \n");
		return 0;
	}
	return board[sq.y][sq.x];
}

int FundamentalBoard::getPieceOnSquare(int sqY, int sqX) const {
	return board[sqY][sqX];
}

set<int> FundamentalBoard::getWhiteAlivePieceSet() const {
	return whiteAlivePieceIdxs;
}

set<int> FundamentalBoard::getBlackAlivePieceSet() const {
	return blackAlivePieceIdxs;
}

set<int> FundamentalBoard::getAlivePieceSet(PlayerColor color) const {
	
	if (color == PlayerColor::WHITE)
		return whiteAlivePieceIdxs;
	else if (color == PlayerColor::BLACK)
		return blackAlivePieceIdxs;
	else
		cout << "getAlivePieceSet: Invalid argument" << endl;
}

// Overloaded method for accesing alive pieces of sertain colour 
// without needing to check every square
// If needing all pieces better to use getBlackAlivePieceSet() and iterate trough
int FundamentalBoard::getPieceOnSquare(PlayerColor player, int i) const {
	int index = 0;
	if (player == PlayerColor::WHITE) {
		if (i < blackAlivePieceIdxs.size())
			// this is a litle slow  
			index = *std::next(blackAlivePieceIdxs.begin(), i);
		Square sq;
		IndexToSquare(index, &sq);
		return getPieceOnSquare(sq);
	}
	return 0;
}

void FundamentalBoard::IndexToSquare(int index, Square* square) {
	// 64 squares, index row by row from top left
	// index 0 is x=0, y=0, index 7 is x7, y=0, index 63 is x=7, y=7
	square->x = index % 8;
	square->y = index / 8;
}

int FundamentalBoard::SquareToIndex(Square square) {
	return SquareToIndex(square.y, square.x);
}

int FundamentalBoard::SquareToIndex(int sqY, int sqX) {
	// 64 squares, index row by row from top left
	// index 0 is x=0, y=0, index 7 is x7, y=0, index 63 is x=7, y=7
	return sqY * 8 + sqX;
}

void FundamentalBoard::SetPieceOnSquare(int piece, Square sq)
{
	board[sq.y][sq.x] = piece;
}

void FundamentalBoard::SetPieceOnSquare(int piece, int sqY, int sqX)
{
	board[sqY][sqX] = piece;
}

int FundamentalBoard::getPieceOnWhiteOutedSquare(int squareNr) const
{
	if (squareNr < outedWhite.size())
		return outedWhite[squareNr];
	else
		return 0;
}

int FundamentalBoard::getPieceOnBlackOutedSquare(int square) const
{
	if (square < outedBlack.size())
		return outedBlack[square];
	else
		return 0;
}

void FundamentalBoard::MovePiece(Square from, Square to) {

	int fromPiece = getPieceOnSquare(from);
	SetPieceOnSquare(fromPiece, to);
	SetPieceOnSquare(0, from);

	if (fromPiece > 0) { // white
		whiteAlivePieceIdxs.erase(SquareToIndex(from));
		whiteAlivePieceIdxs.insert(SquareToIndex(to));
	}
	else if (fromPiece < 0) { // black
		blackAlivePieceIdxs.erase(SquareToIndex(from));
		blackAlivePieceIdxs.insert(SquareToIndex(to));
	}

}

void FundamentalBoard::RemovePiece(Square sq) {

	int piece = getPieceOnSquare(sq);

	if (piece > 0) {
		whiteAlivePieceIdxs.erase(SquareToIndex(sq));
		outedWhite.push_back(piece);
	}
	else if (piece < 0) {
		blackAlivePieceIdxs.erase(SquareToIndex(sq));
		outedBlack.push_back(piece);
	}
}


bool FundamentalBoard::getCastelingPossible(casteling castelingType) const {
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

void FundamentalBoard::setCastelingNotPossible(casteling castelingType) {
	switch (castelingType) {
	case BLACK_LONG:
		 castelingPossible.blackLong = false;
	case BLACK_SHORT:
		castelingPossible.blackShort = false;
	case WHITE_SHORT:
		castelingPossible.whiteShort = false;
	case WHITE_LONG:
		castelingPossible.whiteLong = false;
	default:
		printf("setCastelingNotPossible: illegal argument");
	}
}

