#pragma once
#ifndef _evaluator_h
#define _evaluator_h
#include "source/board/position.h"
#include "source/SharedTypes.h"
#include "source/comon.h"
#include <map>

const float MATERIAL_WEIGHT = 1.0;
const float MOBILITY_WEIGHT = 0.1;
const float QUEEN_MOBILITY_WEIGHT = 2.0;

class Evaluator {
	public:
		// Calculates relative score current player score - opponent score
		static float evaluatePosition(const Position* position, PlayerColor playerColor);
		
private:
	static float countMaterial(const Position* position, PlayerColor side);
	static int countMobilityPlayer(const Position* position, PlayerColor side);
		
};




#endif
