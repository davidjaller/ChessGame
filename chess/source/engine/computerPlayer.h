#pragma once
#ifndef _computer_player_h
#define _computer_player_h

#define MAX_SEARCH_DEPTH 3

#include "source/Player.h"
#include "source/SharedTypes.h"
#include <source/RulesManager.h>
#include "source/io/Scene.h"
#include "source/engine/miniMax.h"
#include <ctime>
#include <chrono>
#include <thread>

class ComputerPlayer : public Player {

public:
	ComputerPlayer(PlayerColor color, Scene* pScene, Board* pBoard);
	bool stepTurn();
	void init();
private:
	float minMaxRecursive(Board board, int level);
	bool findBestMove();
	int nodeCount;
	Move bestMove;
	Scene* scene;
};

#endif
