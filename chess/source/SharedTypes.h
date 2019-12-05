
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
#define PAWN 1
#define BISHOP 2
#define KNIGHT 3
#define ROOK 4
#define QUEEN 5
#define KING 6

// signed pieces

#define WHITE_PAWN = 1
#define BLACK_PAWN = -1
#define WHITE_BISHOP 2
#define BLACK_BISHOP -2
#define WHITE_KNIGHT 3
#define BLACK_KNIGHT -3
#define WHITE_ROOK 4
#define BLACK_ROOK -4
#define WHITE_QUEEN 5
#define BLACK_QUEEN -5
#define WHITE_KING 6
#define BLACK_KING -6

enum casteling { WHITE_SHORT, WHITE_LONG, BLACK_SHORT, BLACK_LONG };

enum class PlayerColor { WHITE = 0, BLACK = 1 };

typedef struct
{
	int pieceType;
	Square square;
}Piece;

typedef struct
{
	Square from;
	Square to;
	int piece;
	bool kill;

}Move;

#endif