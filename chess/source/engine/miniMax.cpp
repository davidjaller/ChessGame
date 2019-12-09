//
//#include "source/engine/miniMax.h"
//
//MinMax::MinMax(PlayerColor color) {
//	level = 0;
//	maximizingPlayer = color;
//	nodeCount = 0;
//}
//void MinMax::initSearch() {
//
//};
//
//
//float MinMax::minMaxRecursive(Board board) {
//	if (level++ >= MAX_SEARCH_DEPTH) {
//		return Evaluator::evaluatePosition(&board, maximizingPlayer);
//	}
//
//	bool maximizing = board.getTurn() == maximizingPlayer;
//
//	Square sq;
//	list<Move> moveList;
//	MoveGenerator::generateAll(&board, moveList);
//	float maxScore = 0;
//	float minScore = std::numeric_limits<float>::max();
//	for (list<Move>::iterator it = moveList.begin(); it != moveList.end(); it++) {
//		Board newBoard = board;
//		newBoard.makeMoveFromTo(it->from, it->to);
//		// is in check? 
//		newBoard.setTurn(getOpposite(board.getTurn()));
//		if (maximizing) {
//			// here if score==maxScore, we could randomize while max levels is low
//			maxScore = max(maxScore, minMaxRecursive(newBoard));
//		}
//		else {
//			minScore = min(minScore, minMaxRecursive(newBoard));
//		}
//		nodeCount++;
//	}
//	if (maximizing)
//		return maxScore;
//	else
//		return minScore;
//
//}
//
//Move MinMax::search(Board* gameBoard, PlayerColor player) {
//	level = 0;
//	maximizingPlayer = player;
//	nodeCount = 0;
//
//	// consider starting this as a thread
//	Board board = *gameBoard; //Needed?
//	float score = minMaxRecursive(board);
//}