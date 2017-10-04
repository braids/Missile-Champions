#include "../Objects.h"

void BoostStreak::SpawnSprite(Assets::Image* sprite, Car* car) {
	this->parent = car;
	this->x = car->x;
	this->y = car->y - car->z;
	for (double a = 11.25, i = 0.0; a <= 371.25; a += 22.5, i++) {
		if (car->angle < a && car->angle >= (a - 22.5)) {
			this->angleSprite = (int)i;
		}
		if (i == 7.0) i = -1.0;
	}
	this->timeAlive = 250;
	this->decaySprite = 0;
	this->image = &sprite[this->angleSprite];
	this->viewportRect = Graphics::CreateRect(this->image->rect->w, this->image->rect->h, 0, 0);
}

void BoostStreak::UpdateDecaySprite(Uint32 timestep) {
	this->timeAlive -= timestep;
	if (this->timeAlive > 100) this->decaySprite = 0;
	else if (this->timeAlive > 50) this->decaySprite = 1;
	else if (this->timeAlive > 0) this->decaySprite = 2;
	if (this->timeAlive < 0) this->timeAlive = 0;
}

void BoostStreak::UpdateViewport(Camera* camera) {
	if (this->timeAlive > 0) {
		this->viewportRect->x = (int)this->x - camera->drawarea->rect->x;
		this->viewportRect->y = (int)this->y - camera->drawarea->rect->y;
	}
}
