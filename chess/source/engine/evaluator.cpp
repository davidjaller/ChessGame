#include "source/engine/evaluator.h"

map<int, float> NOM_PIECE_WEIGHTS =
{{PAWN,   1.0},
{ BISHOP, 3.0 },
{ KNIGHT, 3.0 },
{ ROOK, 5.0 },
{ QUEEN, 9.0 } };

float Evaluator::evaluatePosition(const Position* position, PlayerColor myColor) {

	float materialScore = countMaterial(position, myColor) - countMaterial(position, getOpposite(myColor));
	float mobilityScore = float(countMobilityPlayer(position, myColor) - countMobilityPlayer(position, getOpposite(myColor)));

	// Add up score
	array<array<float, 2>, 2> scores_weights = 
	{ { {materialScore, MATERIAL_WEIGHT}, 
	{	 mobilityScore, MOBILITY_WEIGHT}} };
	float score = 0;
	for (int i = 0; i < scores_weights.size(); i++) {
		score += scores_weights[i][0] * scores_weights[i][1];
	}
	return score;
}

float Evaluator::countMaterial(const Position* position, PlayerColor side) {
	
	float count = 0;
	for (const_iterator_t it = position->piecesBegin(side); it != position->piecesEnd(side); ++it) {
		int piece = it->second.type;
		count += NOM_PIECE_WEIGHTS[abs(piece)];
	}
	return count;
}

 int Evaluator::countMobilityPlayer(const Position* position, PlayerColor side) {
	 bitBoard_t attackedBB = position->getAttackedSquaresBB(getOpposite(side));
	 int count = 0;
	 for (const_iterator_t it = position->piecesBegin(side); it != position->piecesEnd(side); ++it)
	 {
		 count += popCountBB((it->second.canMoveToBB) & ~attackedBB);
	 }
	 return count;
}