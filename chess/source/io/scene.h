///////////////////////////////////////////////////////////////////////////////
// Class Scene
// 
// Prints stuff on the screen
///////////////////////////////////////////////////////////////////////////////

#ifndef _scene_h
#define _scene_h

#pragma once

#include <SDL.h>		
#include <set>
#include "sdl/SDL_gfxprimitives.h"	
#include "source/board/Board.h"

#pragma comment (lib, "../../SDL-1.2.15/lib/x86/SDL.lib")

//#pragma comment (lib, "SDL/SDL_GfxPrimitives/SDL_GfxPrimitives_Static.lib")


// ---- Defines ----
#define BORDER_THICKNESS 2
#define BOARD_LINE_WIDTH 6			// Width of each of the two lines that delimit the board
#define BLOCK_SIZE 16				// Width and Height of each block of a piece (16)
#define SQUARE_SIZE 40
#define BOARD_POSITION 160 			// Center position of the board from the left of the screen (320)
#define BOARD_WIDTH 10				// Board width in blocks 
#define BOARD_HEIGHT 20				// Board height in blocks
#define MIN_VERTICAL_MARGIN 20		// Minimum vertical margin for the board limit 		
#define MIN_HORIZONTAL_MARGIN 20	// Minimum horizontal margin for the board limit
#define PIECE_BLOCKS 5				// Number of horizontal and vertical blocks of a matrix piece

// ------ Enums -----
enum color { BLACK, RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, BROWN, LIGHT, COLOR_MAX }; // Colors
enum corners {LEFT, RIGHT, BOTTOM,TOP};
static SDL_Surface* mScreen;										// Screen
static Uint32 mColors[COLOR_MAX] = { 0x000000ff,					// Colors
							   0xff0000ff, 0x00ff00ff, 0x0000ffff,
							   0x00ffffff, 0xff00ffff, 0xffa000ff,
							   0xffffffff,0x8B4513ff, 0xFFF8C6 };
class Scene
{
public:

	// VARIABLES
	int x1;
	int x2;
	int y1;
	int y2;
	
	// CONSTRUCTOR
	Scene(Board *pBoard);

	//DESTRUCTOR
	~Scene();

	//METHODS
	void DrawChessBoard();
	void UpdateScreen();
	int GetScreenHeight();
	void ClearScreen();
	bool DrawPieces();
	int getBoardBoundry(int boundry);
	void MarkSquare(Square square);
	void CreateScene();

private:

	// VARIABLES
	Board *gameBoard;
	SDL_Surface* bitmap;

	//METHODS
	int InitGraph();
	void DrawRectangle (int pX1, int pY1, int pX2, int pY2, enum color pC);
	SDL_Surface* CreateSurface(int width , int height);
	void PutPiece(int x, int y, int piece);

};

#endif // !_scene_h

