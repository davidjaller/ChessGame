#ifndef _move_visualizer_player_h
#define _move_visualizer_player_h
#pragma once

#include "source/Player.h"
#include "source/SharedTypes.h"
#include <source/RulesManager.h>
#include "source/io/Scene.h"
#include <time.h>
#ifndef __linux__
#include <windows.h>
#endif

class MoveVisualizerPlayer : public Player {
public:
	MoveVisualizerPlayer(PlayerColor color, Scene* pScene, Position* pPosition);
	void init();
	bool stepTurn();
private:

	Position tempPosition; 
	Scene* myScene;
	int state;

	bool turnDone;
};
#endif