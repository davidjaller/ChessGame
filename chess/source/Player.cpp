#include "Player.h"

Player::Player(Board* pBoard, PlayerColor color) {
	gameBoard = pBoard;
	SetColor(color);
}

Player::~Player() {}

void Player::SetColor(PlayerColor color) {
	this->color = color;
}

PlayerColor Player::GetColor() {
	return color;
}

bool Player::isChecked() {
	if (!kingAttackers.empty())
		kingAttackers.clear();
	
	bool checked = RulesManager::KingIsChecked(gameBoard, &kingAttackers, color);

	return checked;
}

bool Player::generateMoves(){
	resetMoves();


	int count = 0;
	set<int> myPieces = gameBoard->getAlivePieceSet(color);
	Square sq;
	for (set<int>::iterator it = myPieces.begin(); it != myPieces.end(); ++it) {
		Board::IndexToSquare(*it, &sq);
		count += MoveGenerator::generate(gameBoard, sq, gameBoard->getPieceOnSquare(sq), &possibleMoves);
	}
	return count > 0;
}

void Player::resetMoves() {
	possibleMoves.clear();
}



bool Player::canMove() {
	return false;
}

