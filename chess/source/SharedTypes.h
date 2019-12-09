
///////////////////////////////////////////////////////////////////////////////
// Shared Constants
// Types and constants needed in more than one file
///////////////////////////////////////////////////////////////////////////////


#pragma once
#ifndef _shared_const_h_
#define _shared_const_h_
#include <list>
typedef struct
{
	int file;
	int rank;
}Square;

typedef struct
{
	bool whiteLong;
	bool whiteShort;
	bool blackLong;
	bool blackShort;
}BoolStruct;

// unsigned pieces
const int PAWN = 1;
const int BISHOP = 2;
const int KNIGHT = 3;
const int ROOK = 4;
const int QUEEN = 5;
const int KING = 6;

// signed pieces

const int WHITE_PAWN = 1;
const int BLACK_PAWN = -1;
const int WHITE_BISHOP = 2;
const int BLACK_BISHOP = 2;
const int WHITE_KNIGHT = 3;
const int BLACK_KNIGHT = 3;
const int WHITE_ROOK = 4;
const int BLACK_ROOK = 4;
const int WHITE_QUEEN = 5;
const int BLACK_QUEEN = 5;
const int WHITE_KING = 6;
const int BLACK_KING = 6;

enum casteling { WHITE_SHORT, WHITE_LONG, BLACK_SHORT, BLACK_LONG };

enum class PlayerColor { WHITE = 0, BLACK = 1 };

typedef struct
{
	Square from;
	Square to;
	int piece;
	bool kill;

}Move;

#endif