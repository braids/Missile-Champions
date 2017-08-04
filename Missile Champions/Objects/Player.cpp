#include "../Objects.h"

void Player::Init(Assets* assets) {
	this->team = 0;
	this->score = 0;
	this->selectionViewport = Graphics::CreateRect(
		assets->images.CarSelectWindowSprites[0].rect->w, 
		assets->images.CarSelectWindowSprites[0].rect->h, 
		0, 8);
	this->activeCar = &this->cars[0];

	for (int i = 0; i < 3; i++) this->cars[i].InitCar(assets);
}

void Player::SetCarSelection(int player_num) {
	this->team = 0;
	this->activeCar = &this->cars[0];
	this->activeCar->SetCarSelect(player_num);
}

void Player::SetKickoff(int player_num) {
	for (int i = 0; i < 3; i++) this->cars[i].SetCarKickoff(player_num, i);
	this->score = 0;
}