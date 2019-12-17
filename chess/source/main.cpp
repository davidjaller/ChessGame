/*****************************************************************************************
Program: Chess Game

Author: David Jaller
/*****************************************************************************************/

#include "source/io/scene.h"
#include "source/Game.h"
#ifndef LINUX
#include <windows.h>
#endif

#define DEBUG_MODE

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
#ifdef DEBUG_MODE
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
#endif
	
	Position position = Position(); 
	Scene scene(&position);
	Game game(&scene, &position);

	scene.CreateScene();
	scene.UpdateScreen();
	srand(time(NULL));
	game.NewGame();
	bool gameFinnished = false;
	// ----- Main Loop -----
	
	while(!IsKeyDown (SDLK_ESCAPE))
	{
		if (!gameFinnished)
			gameFinnished = game.StepGame();

		scene.UpdateScreen();
		if(IsKeyDown(SDLK_F1))
		{
			game.NewGame();
		}
	}
	SDL_Quit();
	return 0;
}


