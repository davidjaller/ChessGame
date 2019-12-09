#include "source/engine/evaluator.h"

//float NOM_PIECE_WEIGHTS[5] = {1.0, 3.0, 3.0, 5.0, 9.0};
//TODO: make the following work, since more clear
map<int, float> NOM_PIECE_WEIGHTS =
{ {PAWN,   1.0},
{ BISHOP, 3.0 },
{ KNIGHT, 3.0 },
{ ROOK, 5.0 },
{ QUEEN, 9.0 } };


float Evaluator::evaluatePosition(const Board* board, PlayerColor myColor) {

	set<int> myPieceSet = *(board->getAlivePieceSet(myColor));
	set<int> oponentPieceSet = *(board->getAlivePieceSet(getOpposite(myColor)));


	float materialScore = countMaterial(board, myPieceSet) - countMaterial(board, oponentPieceSet);
	float mobilityScore = 0;

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


float Evaluator::countMaterial(const Board* board, set<int> pieceSet) {
	
	float count = 0;
	for (set<int>::iterator it = pieceSet.begin(); it != pieceSet.end(); ++it) {
		int piece = board->getPieceOnSquare(Board::IndexToSquare(*it));
		count += NOM_PIECE_WEIGHTS[abs(piece)];
	}
	return count;
}
float Evaluator::countMobilityPiece(const Board* board, Square pieceSquare) {
	return 0;
}
 float Evaluator::countMobilityPlayer(const Board* board, set<int> pieceSet) {
	 return 0;
}