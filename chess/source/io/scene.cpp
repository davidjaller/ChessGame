///////////////////////////////////////////////////////////////////////////////
// Class Scene
// 
// Prints stuff on the screen
///////////////////////////////////////////////////////////////////////////////

// -- Includes ---

#include "source/io/scene.h"
#include "windows.h"
#include <stdio.h>


///////////////////////////////////////////////////////////////////////////////
// Constructor
//
// 
///////////////////////////////////////////////////////////////////////////////

Scene::Scene(Board *pBoard)
{
	gameBoard = pBoard;
	InitGraph ();
}

Scene::~Scene() {
}

///////////////////////////////////////////////////////////////////////////////
// UpdateScreen
///////////////////////////////////////////////////////////////////////////////
void Scene::UpdateScreen()
{
	SDL_Flip(mScreen);
}

///////////////////////////////////////////////////////////////////////////////
// GetScreenHeight
///////////////////////////////////////////////////////////////////////////////
int Scene::GetScreenHeight()
{
	return mScreen->h;
}

///////////////////////////////////////////////////////////////////////////////
// ClearScreen
///////////////////////////////////////////////////////////////////////////////
void Scene::ClearScreen() 
{
	boxColor (mScreen, 0, 0, mScreen->w - 1, mScreen->h - 1, mColors[YELLOW]);
}

///////////////////////////////////////////////////////////////////////////////
// DrawRecangle
///////////////////////////////////////////////////////////////////////////////
void Scene::DrawRectangle (int pX1, int pY1, int pX2, int pY2, enum color pC)
{
	boxColor (mScreen, pX1, pY1, pX2, pY2-1, mColors[pC]);
}

///////////////////////////////////////////////////////////////////////////////
// DrawChessBoard
//
///////////////////////////////////////////////////////////////////////////////
void Scene::DrawChessBoard()
{
	x1 = 640/2-SQUARE_SIZE*4;
	y1 = 480/2-SQUARE_SIZE*4;
	x2 = 640/2+SQUARE_SIZE*4;
	y2 = 480/2+SQUARE_SIZE*4;

	DrawRectangle(x1- BORDER_THICKNESS, y1- BORDER_THICKNESS, x2+ BORDER_THICKNESS, y2+ BORDER_THICKNESS, BLACK);

	color col;

	for (int i = 0; i < 8;i++)
	{
		for(int j = 0; j<8;j++)
		{
			if ((i+j) % 2 == 0)
			{
				col = WHITE;
			}
			else
			{
				col = BROWN;
			}	
			DrawRectangle(x1 + i * SQUARE_SIZE, y1 + j * SQUARE_SIZE,x1 + (i+1) * SQUARE_SIZE,y1 + (j+1) * SQUARE_SIZE,col);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// getBoarderBoundry
//
///////////////////////////////////////////////////////////////////////////////
int Scene::getBoardBoundry(int boundry)
{
	switch(boundry)
	{
		case RIGHT: return x2;
		case BOTTOM: return y2;
		case TOP: return y1;
		case LEFT: return x1;
		default: return 0;
	}
}

///////////////////////////////////////////////////////////////////////////////
// MarkSquare
//
// Changes color or puts boarder on specified square
///////////////////////////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////////////////////////
void Scene::MarkSquare(Square square)
{
		SDL_Surface* s = CreateSurface(SQUARE_SIZE ,SQUARE_SIZE ) ;
        ( void )SDL_FillRect( s , NULL , 0xAA0000FF ) ;

        SDL_Rect rect = { SQUARE_SIZE , SQUARE_SIZE } ;

        rect.x = x1 + square.file * SQUARE_SIZE ;
        rect.y = y1 + square.rank *SQUARE_SIZE ;
        ( void )SDL_FillRect( s , NULL , 0x440000FF ) ;
        ( void )SDL_BlitSurface( s , NULL , mScreen , &rect ) ;
		SDL_FreeSurface( s ) ;
}

SDL_Surface* Scene:: CreateSurface( int width , int height )
{
    uint32_t rmask , gmask , bmask , amask ;

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
    #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
    #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
    #endif

    SDL_Surface* surface = SDL_CreateRGBSurface( 0 , width , height , 32 , rmask , gmask , bmask , amask ) ;
    if( surface == NULL ) 
    {
        ( void )fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError() );
        exit(1);
    }

return surface ;
}

///////////////////////////////////////////////////////////////////////////////
// Draw Pieces
//
///////////////////////////////////////////////////////////////////////////////
bool Scene::DrawPieces()
{
	int piece, file, rank;
	vector<PlayerColor> white_black = { PlayerColor::WHITE, PlayerColor::BLACK};
	for (int i = 0; i < 2; i++) {
		Square sq;

		set<int> pieceSet = gameBoard->getAlivePieceSet(white_black[i]);
		for (set<int>::iterator it = pieceSet.begin(); it != pieceSet.end(); ++it) {

			Board::IndexToSquare(*it, &sq);
			piece = gameBoard->getPieceOnSquare(sq);
			file = x1 + SQUARE_SIZE * sq.file;
			rank = y1 + SQUARE_SIZE * sq.rank;
			PutPiece(file, rank, piece);

		}
	}

	int i = 0;
	while (piece = gameBoard->getPieceOnWhiteOutedSquare(i))
	{
		if (i >= 8)
		{
			rank = y2 + SQUARE_SIZE * 2;
			file = x1 + SQUARE_SIZE * (i-7);
		}
		else
		{
			file = x1 + SQUARE_SIZE * (i);
			rank = y2 + SQUARE_SIZE * 1;
		}
	
		PutPiece(file,rank, piece);
		i++;
	}
	i = 0;
	while(piece = gameBoard->getPieceOnBlackOutedSquare(i))
	{
		
		if (i >= 8)
		{
			rank = y1 - SQUARE_SIZE * 1;
			file = x1 + SQUARE_SIZE * (i-7);
		}
		else
		{
			file = x1 + SQUARE_SIZE * (i);
			rank = y1 - SQUARE_SIZE * 2;
		}
		PutPiece(file,rank, piece);
		i++;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// PutPiece
//
// Adds image of piece
///////////////////////////////////////////////////////////////////////////////
void Scene::PutPiece(int file, int rank, int piece)
{
			SDL_Rect source;
			source.x = 0;
			source.y = 0;
			source.w = 100;
			source.h = 100;

			SDL_Rect destination;
			destination.x = file;
			destination.y = rank;
			destination.w = SQUARE_SIZE;
			destination.h = SQUARE_SIZE;
		
			SDL_Surface* bitmap;
			if(piece!=0)
			{
				switch (piece)
				{
				case -1:  bitmap = SDL_LoadBMP("pieces/bP.bmp");	
					break;
				case -4:  bitmap = SDL_LoadBMP("pieces/bR.bmp");
					break;
				case -3:  bitmap = SDL_LoadBMP("pieces/bKn.bmp");
					break;
				case -2:  bitmap = SDL_LoadBMP("pieces/bB.bmp");
					break;
				case -5:  bitmap = SDL_LoadBMP("pieces/bQ.bmp");
					break;
				case -6:  bitmap = SDL_LoadBMP("pieces/bK.bmp");
					break;
				case 1: bitmap = SDL_LoadBMP("pieces/wP.bmp");
					break;
				case 4:  bitmap = SDL_LoadBMP("pieces/wR.bmp");
					break;
				case 3:  bitmap = SDL_LoadBMP("pieces/wKn.bmp");
					break;
				case 2:  bitmap = SDL_LoadBMP("pieces/wB.bmp");
					break;
				case 5:  bitmap = SDL_LoadBMP("pieces/wQ.bmp");
					break;
				case 6:  bitmap = SDL_LoadBMP("pieces/wK.bmp");
					break;
				default:  bitmap = SDL_LoadBMP("pieces/wK.bmp");
					break;
				}

				// Make white color of image transparant
				SDL_SetColorKey( bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(bitmap->format, 255, 0, 0) );

				SDL_BlitSurface(bitmap,&source,mScreen,&destination);
				SDL_FreeSurface(bitmap);
			}
}

///////////////////////////////////////////////////////////////////////////////
// CreateScene
//
///////////////////////////////////////////////////////////////////////////////

void Scene::CreateScene()
{
	ClearScreen();
	DrawChessBoard();
	DrawPieces();
}


///////////////////////////////////////////////////////////////////////////////
// InitGraph
//
///////////////////////////////////////////////////////////////////////////////
int Scene::InitGraph()
{
	const SDL_VideoInfo *info;
	Uint8  video_bpp;
	Uint32 videoflags;
        
	// Initialize SDL
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n",SDL_GetError());
		return 1;
	}
	atexit(SDL_Quit);
	

	// Alpha blending doesn't work well at 8-bit color
	info = SDL_GetVideoInfo();
	if ( info->vfmt->BitsPerPixel > 8 ) {
		video_bpp = info->vfmt->BitsPerPixel;
	} else {
		video_bpp = 16;
	}
	videoflags = SDL_SWSURFACE | SDL_DOUBLEBUF;
	
	// Set 640x480 video mode
	if ( (mScreen=SDL_SetVideoMode(640,480,video_bpp,videoflags)) == NULL ) {
		fprintf(stderr, "Couldn't set %ix%i video mode: %s\n",640,480,SDL_GetError());
		return 2;
	}
    return 0;
}




