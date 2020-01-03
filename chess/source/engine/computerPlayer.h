#pragma once
#ifndef _computer_player_h
#define _computer_player_h

#define MAX_SEARCH_DEPTH 1

#include "source/Player.h"
#include "source/SharedTypes.h"
#include <source/RulesManager.h>
#include "source/io/Scene.h"
#include "source/engine/miniMax.h"
#include <ctime>
#include <chrono>
#include <thread>
#include "debug.h"

class ComputerPlayer : public Player {

public:
	ComputerPlayer(PlayerColor color, Scene* pScene, Position* pPosition);
	bool stepTurn();
	void init();
private:
	float minMaxRecursive(Position position, Move move, int level);
	bool findBestMove();
	int nrEvaluatedPositions;
	Move bestMove;
	Scene* scene;
};

#endif
