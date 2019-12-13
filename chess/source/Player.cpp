#include "Player.h"

Player::Player(Position* pPosition, PlayerColor color) {
	gamePosition = pPosition;
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
	
	bool checked = RulesManager::KingIsChecked(gamePosition, &kingAttackers, color);

	return checked;
}

void Player::generateMoves(){
	resetMoves();
	MoveGenerator::generateAll(gamePosition, &possibleMoves);
}

void Player::resetMoves() {
	possibleMoves.clear();
}

bool Player::canMove() {
	resetMoves();
	return MoveGenerator::generateAll(gamePosition, &possibleMoves) > 0;
}

