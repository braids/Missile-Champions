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