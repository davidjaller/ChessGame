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
	
	bool checked = gamePosition->KingIsChecked(&kingAttackers, color);

	return checked;
}

void Player::generateMoves(){
	resetMoves();
	//MoveGenerator::generateMoves(gamePosition, &possibleMoves, true); // captures
	MoveGenerator::generateMoves(gamePosition, &possibleMoves, false); //pushes
}

void Player::resetMoves() {
	possibleMoves.clear();
}

bool Player::canMove() {
	generateMoves();
	return possibleMoves.size() > 0;
}

