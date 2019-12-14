#pragma once
#ifndef _evaluator_h
#define _evaluator_h
#include "source/board/position.h"
#include "source/SharedTypes.h"
#include "source/comon.h"
#include <map>

const float MATERIAL_WEIGHT = 1.0;
const float MOBILITY_WEIGHT = 1.0;
const float QUEEN_MOBILITY_WEIGHT = 2.0;

class Evaluator {
	public:
		// Calculates relative score current player score - opponent score
		static float evaluatePosition(const Position* position, PlayerColor playerColor);
		
private:
	static float countMaterial(const Position* position, PlayerColor side);
	static float countMobilityPiece(const Position* position, Square pieceSquare);
	static float countMobilityPlayer(const Position* position, set<int> pieceSet);
		
};




#endif
