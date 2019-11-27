/*****************************************************************************************
/* File: Board.cpp

/*
/*****************************************************************************************/

// ----- Includes -----

#include "source/Board.h"
#include "source/Game.h"
#include "windows.h"
#include <stdio.h>
#include <math.h>
#include <string>
using namespace std;



int initBoard[8][8] = {
						{-4, -3, -2, -5, -5, -2, -3, -4},
						{-1, -1, -1, -1, 0, -1, -1, -1},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 0,  0,  0,  0,  0,  0,  0,  0},
						{ 1,  1,  1,  1,  0,  1,  1,  1},
						{ 4,  3,  2,  5,  6,  2,  3,  4}
						};

//char board[8][8];
/*
==================
Init
==================
*/
Board::Board ()
{
	InitBoard();
}

void Board::InitBoard()
{
	
	for(int i = 0; i<8;i++)
	{
		for(int j = 0; j < 8;j++)
		{
			board[i][j]=initBoard[i][j];
		}
	}
	
	whiteKing.x = 4;
	whiteKing.y = 7;
	blackKing.x = 4;
	blackKing.y = 0;
	castelingPossible.blackLong = 1;
	castelingPossible.blackShort = 1;
	castelingPossible.whiteLong = 1;
	castelingPossible.whiteShort = 1;
	outedCountWhite = 0;
	outedCountBlack = 0;
}

Square Board::GetKingPos(Player pl)
{
	
	if (pl == Player::WHITE)
		return whiteKing;
	else
		return blackKing;
}

void Board::SetKingPos(Square sq)
{
	if (Game::getPlayer() == Player::WHITE)
	{
		whiteKing.x = sq.x;
		whiteKing.y = sq.y;
	}
	else
	{
		blackKing.x = sq.x;
		blackKing.y = sq.y;
	}
		
}

int Board::getPieceOnSquare(Square sq)
{
	return board[sq.y][sq.x];
}

void Board::SetPieceOnSquare(int piece, Square sq)
{
	board[sq.y][sq.x] = piece;
}

int Board::getPieceOnWhiteOutedSquare(int square)
{
	return outedWhite[square];
}

int Board::getPieceOnBlackOutedSquare(int square)
{
	return outedBlack[square];
}


void Board::makeMoveFromTo(Square from, Square to)
{
	Player player = Game::getPlayer();
	// If rook moves update casteling posablity
	if((castelingPossible.whiteShort) && board[from.y][from.x] == WHITE_ROOK && from.y == 0)
		castelingPossible.whiteShort = false;

	else if((castelingPossible.whiteLong) && board[from.y][from.x] == WHITE_ROOK && from.x == 7)
		castelingPossible.whiteLong = false;

	else if((castelingPossible.blackShort) && board[from.y][from.x] == BLACK_ROOK && from.x == 0)
		castelingPossible.blackShort = false;

	else if((castelingPossible.blackLong) && board[from.y][from.x] == BLACK_ROOK && from.x == 7)
		castelingPossible.blackLong = false;

	// king move
	else if (abs(board[from.y][from.x]) == KING)
	{
		// Check if a casteling move and move the rook
		if (from.x == 4 && POV(from.y) == 0 && POV(to.y) == 0 && (to.x == 2 || to.x == 6)) {
			if (to.x == 2) { // long
				board[from.y][3] = 4 * (1 - 2 * (int)player);
				board[from.y][0] = 0;
			}
			else if (to.x == 6) { // short
				board[from.y][5] = 4 * (1 - 2 * (int)player);
				board[from.y][7] = 0;
			}
		}
		
		// if king move then casteling not possible in future
		if(player == Player::WHITE)
		{
			castelingPossible.whiteLong = false;
			castelingPossible.whiteShort = false;
		}
		else if(player == Player::BLACK)
		{
			castelingPossible.blackLong = false;
			castelingPossible.blackShort = false;
		}
		SetKingPos(to);
	}
	

	if (board[to.y][to.x] > 0)
	{
		outedWhite[outedCountWhite++] = board[to.y][to.x];
	}

	else if (board[to.y][to.x] < 0)
	{
		outedBlack[outedCountBlack++] = board[to.y][to.x];

	}

	// Update the to and from squares
	board[to.y][to.x] = board[from.y][from.x];
	board[from.y][from.x] = 0;

	//pawn promotion
	if(abs(board[to.y][to.x]) == 1 && (to.y == 0 || to.y == 7 ) )
	{
		board[to.y][to.x] = QUEEN * (1 - 2*(int)player);
	}
}


bool Board::IsEmptySquare(Square sq)
{
	if (board[sq.y][sq.x]==0)
		return true;
	else
		return false;
}


 int Board::POV(int rankNumber){
	if(Game::getPlayer()==Player::BLACK)
		return rankNumber;
	else 
		return 7-rankNumber;
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

bool Board:: IsFriendlyPiece(int piece)
{
	if( (Game::getPlayer() == Player::WHITE && piece > 0) 
		|| (Game::getPlayer() == Player::BLACK && piece < 0) )
		return true;
	else
		return false;
}

bool Board::IsFriendlyPiece(Square square)
{

	int piece = getPieceOnSquare(square); 

	return IsFriendlyPiece(piece);
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
