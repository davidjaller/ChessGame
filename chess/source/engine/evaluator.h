#pragma once
#ifndef _evaluator_h
#define _evaluator_h
#include "source/board/board.h"
#include "source/SharedTypes.h"
#include "source/comon.h"
#include <map>




const float MATERIAL_WEIGHT = 1.0;
const float MOBILITY_WEIGHT = 1.0;
const float QUEEN_MOBILITY_WEIGHT = 2.0;

class Evaluator {
	public:
		// Calculates relative score current player score - opponent score
		static float evaluatePosition(const Board* board, PlayerColor playerColor);
		
private:
	static float countMaterial(const Board* board, set<int> pieceSet);
	static float countMobilityPiece(const Board* board, Square pieceSquare);
	static float countMobilityPlayer(const Board* board, set<int> pieceSet);
		
};




#endif
