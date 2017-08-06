#include "../Objects.h"

void Player::Init(Assets* assets, int p_num) {
	this->player_num = p_num;
	this->team = 0;
	this->score = 0;
	this->selectionViewport = Graphics::CreateRect(
		assets->images.CarSelectWindowSprites[0].rect->w, 
		assets->images.CarSelectWindowSprites[0].rect->h, 
		0, 8);
	this->activeCar = &this->cars[0];

	for (int i = 0; i < 3; i++) this->cars[i].InitCar(assets, this);
}

void Player::SetCarSelection() {
	this->team = 0;
	this->activeCar = &this->cars[0];
	this->activeCar->SetCarSelect();
}

void Player::SetStartRound() {
	this->SetKickoff();
	this->score = 0;
}

void Player::SetKickoff() {
	for (int i = 0; i < 3; i++) this->cars[i].SetCarKickoff(i);
}