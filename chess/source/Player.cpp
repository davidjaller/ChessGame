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

