/*****************************************************************************************
Program: Chess Game

Author: David Jaller
/*****************************************************************************************/

#include "source/io/scene.h"
#include "source/Game.h"
#ifndef LINUX
#include <windows.h>
#endif

#include <fstream>
#include <iostream>
#include <stdio.h>
using namespace std;

///////////////////////////////////////////////////////////////////////////////
// IsKeyDown
//
///////////////////////////////////////////////////////////////////////////////

int IsKeyDown (int pKey)
{
	Uint8* mKeytable;
	int mNumkeys;
	SDL_PumpEvents();
	mKeytable = SDL_GetKeyState(&mNumkeys);
	return mKeytable[pKey];
}


///////////////////////////////////////////////////////////////////////////////
// Main
//
///////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain (_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	Board board = Board(PlayerColor::WHITE); 
	Scene scene = Scene(&board);
	Game game = Game(&scene, &board);

	scene.CreateScene();
	scene.UpdateScreen();
	
	// ----- Main Loop -----
	
	while(!IsKeyDown (SDLK_ESCAPE))
	{
		game.GameStep();
		scene.UpdateScreen();
		if(IsKeyDown(SDLK_F1))
		{
			game.NewGame();
		}
	}
	SDL_Quit();
	return 0;
}


