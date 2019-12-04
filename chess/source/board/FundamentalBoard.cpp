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
	for (int rank = 0; rank < 8; rank++)
	{
		for (int file = 0; file < 8; file++)
		{
			board[rank][file] = initMatrix[rank][file];
			if (board[rank][file] > 0) {
				whiteAlivePieceIdxs.insert(SquareToIndex(rank, file));
				if (board[rank][file] == WHITE_KING) {
					whiteKing.file = file;
					whiteKing.rank = rank;
				}
			}
			else if (board[rank][file] < 0) {
				blackAlivePieceIdxs.insert(SquareToIndex(rank, file));
				if (board[rank][file] == BLACK_KING) {
					blackKing.file = file;
					blackKing.rank = rank;
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
	if (sq.file > 7 || sq.rank > 7 || sq.file < 0 || sq.rank < 0) {
		(void)fprintf(stderr, "iligal square : \n");
		return 0;
	}
	return board[sq.rank][sq.file];
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
PlayerColor FundamentalBoard::getOpposite(PlayerColor color) {
	if (color == PlayerColor::WHITE)
		return PlayerColor::BLACK;
	if (color == PlayerColor::BLACK)
		return PlayerColor::WHITE;
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
	// index 0 is file=0, rank=0, index 7 is x7, rank=0, index 63 is file=7, rank=7
	square->file = index % 8;
	square->rank = index / 8;
}

int FundamentalBoard::SquareToIndex(Square square) {
	return SquareToIndex(square.rank, square.file);
}

int FundamentalBoard::SquareToIndex(int sqY, int sqX) {
	// 64 squares, index row by row from top left
	// index 0 is file=0, rank=0, index 7 is x7, rank=0, index 63 is file=7, rank=7
	return sqY * 8 + sqX;
}

void FundamentalBoard::SetPieceOnSquare(int piece, Square sq)
{
	if (sq.rank >= 0 && sq.rank < 8 && sq.file >= 0 && sq.file < 8) {
		board[sq.rank][sq.file] = piece;
	}
	else
		cout << "SetPieceOnSquare(): Out of bounds" << endl;
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

