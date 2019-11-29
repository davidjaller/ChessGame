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

int INIT_BOARD[8][8] = {
						{-4, -3, -2, -5, -6, -2, -3, -4},
						{-1, -1, -1, -1, -1, -1, -1, -1},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 1,  1,  1,  1,  0,  1,  1,  1},
						{ 4,  3,  2,  5,  6,  2,  3,  4}
};

Board::Board (PlayerColor turn)
{
	InitBoard(INIT_BOARD, turn);
}

Board::~Board(){}

// This is usefull for unit testing
void Board::InitBoard(int initMatrix[8][8], PlayerColor turn)
{	
	for(int y = 0; y<8;y++)
	{
		for(int x = 0; x < 8;x++)
		{
			board[y][x]= initMatrix[y][x];
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

Square Board::GetKingPos(PlayerColor pl)
{
	
	if (pl == PlayerColor::WHITE)
		return whiteKing;
	else
		return blackKing;
}

void Board::SetKingPos(Square sq)
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

bool Board::IsEmptySquare(Square sq)
{
	if (getPieceOnSquare(sq) == 0)
		return true;
	else
		return false;
}


int Board::POV(int rankNumber) {
	if (turn == PlayerColor::BLACK)
		return rankNumber;
	else
		return 7 - rankNumber;
}

bool Board::IsFriendlyPiece(int piece)
{
	if ((turn == PlayerColor::WHITE && piece > 0)
		|| (turn == PlayerColor::BLACK && piece < 0))
		return true;
	else
		return false;
}

bool Board::IsFriendlyPiece(Square square)
{
	int piece = getPieceOnSquare(square);
	return IsFriendlyPiece(piece);
}

int Board::getPieceOnSquare(Square sq)
{
	if (sq.x > 7 || sq.y > 7 || sq.x < 0 || sq.y < 0) {
		(void)fprintf(stderr, "iligal square : \n" );
		return 0;
	}
	return board[sq.y][sq.x];
}

int Board::getPieceOnSquare(int sqY, int sqX) {
	return board[sqY][sqX];
}

set<int> Board::getWhiteAlivePieceSet() {
	return whiteAlivePieceIdxs;
}

set<int> Board::getBlackAlivePieceSet() {
	return blackAlivePieceIdxs;
}

// Overloaded method for accesing alive pieces of sertain colour 
// without needing to check every square
// If needing all pieces better to use getBlackAlivePieceSet() and iterate trough
int Board::getPieceOnSquare(PlayerColor player, int i) {
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

 void Board::IndexToSquare(int index, Square* square) {
	 // 64 squares, index row by row from top left
	 // index 0 is x=0, y=0, index 7 is x7, y=0, index 63 is x=7, y=7
	 square->x = index % 8;
	 square->y = index / 8;
}

 int Board::SquareToIndex(Square square) {
	 return SquareToIndex(square.y, square.x);
 }

 int Board::SquareToIndex(int sqY, int sqX) {
	 // 64 squares, index row by row from top left
	 // index 0 is x=0, y=0, index 7 is x7, y=0, index 63 is x=7, y=7
	 return sqY * 8 + sqX;
 }

void Board::SetPieceOnSquare(int piece, Square sq)
{
	board[sq.y][sq.x] = piece;
}

void Board::SetPieceOnSquare(int piece, int sqY, int sqX)
{
	board[sqY][sqX] = piece;
}

int Board::getPieceOnWhiteOutedSquare(int squareNr)
{
	if (squareNr < outedWhite.size())
		return outedWhite[squareNr];
	else
		return 0;
}

int Board::getPieceOnBlackOutedSquare(int square)
{
	if (square < outedBlack.size())
		return outedBlack[square];
	else
		return 0;
}

void Board::MovePiece(Square from, Square to){

	int fromPiece = getPieceOnSquare(from);
	SetPieceOnSquare(fromPiece, to);
	SetPieceOnSquare(0, from);
	if (fromPiece > 0){ // white
		whiteAlivePieceIdxs.erase(SquareToIndex(from));
		whiteAlivePieceIdxs.insert(SquareToIndex(to));
	}
	else if (fromPiece < 0) { // black
		blackAlivePieceIdxs.erase(SquareToIndex(from));
		blackAlivePieceIdxs.insert(SquareToIndex(to));
	}
		
}
void Board::MovePieceKill(Square from, Square to) {

	RemovePiece(to);
	MovePiece(from, to);
}

void Board::RemovePiece(Square sq) {

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
void Board::PromoteQueen(Square square) {

	int piece = getPieceOnSquare(square);
	RemovePiece(square);

	// As long as our whiteAlivePieceIdxs list doesnt 
	// hold the piecetype we dont need to update it here
	if (piece > 0) {
		SetPieceOnSquare(WHITE_QUEEN, square);
	}
	else if (piece < 0)
		SetPieceOnSquare(BLACK_QUEEN, square);
}

void Board::makeMoveFromTo(Square from, Square to)
{
	int fromPiece = getPieceOnSquare(from);
	int toPiece = getPieceOnSquare(to);

	// If rook moves update casteling posablity
	if((castelingPossible.whiteShort) && fromPiece == WHITE_ROOK && from.y == 0)
		castelingPossible.whiteShort = false;

	else if((castelingPossible.whiteLong) && fromPiece == WHITE_ROOK && from.x == 7)
		castelingPossible.whiteLong = false;

	else if((castelingPossible.blackShort) && fromPiece == BLACK_ROOK && from.x == 0)
		castelingPossible.blackShort = false;

	else if((castelingPossible.blackLong) && fromPiece == BLACK_ROOK && from.x == 7)
		castelingPossible.blackLong = false;

	// king move
	else if (abs(getPieceOnSquare(from)) == KING)
	{
		// Check if a casteling move and move the rook
		if (from.x == 4 && POV(from.y) == 0 && POV(to.y) == 0 && (to.x == 2 || to.x == 6)) {
			Square rookFrom = from;
			Square rookTo = to;
			if (to.x == 2) { // long
				rookFrom.x = 0;
				rookTo.x = 3;
				SetPieceOnSquare(ROOK * (1 - 2 * (int)turn), from.y, 3);
				SetPieceOnSquare(0, from.y, 0);
			}
			else if (to.x == 6) { // short
				rookFrom.x = 7;
				rookTo.x = 5;
				SetPieceOnSquare(ROOK * (1 - 2 * (int)turn), from.y, 5);
				SetPieceOnSquare(0, from.y, 7);
			}
		}
		
		// if king move then casteling not possible in future
		if(turn == PlayerColor::WHITE)
		{
			castelingPossible.whiteLong = false;
			castelingPossible.whiteShort = false;
		}
		else if(turn == PlayerColor::BLACK)
		{
			castelingPossible.blackLong = false;
			castelingPossible.blackShort = false;
		}
		SetKingPos(to);
	}
	if (!IsEmptySquare(to))
		MovePieceKill(from, to);
	else
		MovePiece(from, to);

	//pawn promotion
	if(abs(fromPiece) == PAWN && (to.y == 0 || to.y == 7 ) )
	{
		PromoteQueen(to);
	}
}

 bool Board::getCastelingPossible(casteling castelingType) {
	 switch(castelingType) {
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

void Board:: PrintInt(int var, string varName)
{
	char str[255];
	for(int i = 0; i<varName.size() ; i++)
	{
		str[i] = varName[i];
	}
	OutputDebugString(varName.c_str());
	sprintf_s(str, ":  ");
	OutputDebugString(str);
	sprintf_s(str, "%i\n",var);
	OutputDebugString(str);
}
