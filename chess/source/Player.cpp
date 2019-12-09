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

void Player::generateMoves(){
	resetMoves();
	MoveGenerator::generateAll(gameBoard, &possibleMoves);
}

void Player::resetMoves() {
	possibleMoves.clear();
}

bool Player::canMove() {
	resetMoves();
	return MoveGenerator::generateAll(gameBoard, &possibleMoves) > 0;
}

