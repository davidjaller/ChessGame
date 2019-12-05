#ifndef _move_visualizer_player_h
#define _move_visualizer_player_h
#pragma once

#include "source/Player.h"
#include "source/SharedTypes.h"
#include <source/RulesManager.h>
#include "source/io/Scene.h"
#include <time.h>
#include <windows.h>

class MoveVisualizerPlayer : public Player {
public:
	MoveVisualizerPlayer(PlayerColor color, Scene* pScene, Board* pBoard);
	void init();
	bool stepTurn();
private:

	Board tempBoard; 
	Scene* myScene;
	int state;

	bool turnDone;
};
#endif