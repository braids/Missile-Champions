#include "../Objects.h"

void BoostStreak::SpawnSprite(double _x, double _y, double _angle, Assets* assets) {
	this->x = _x;
	this->y = _y;
	for (double a = 11.25, i = 0.0; a <= 371.25; a += 22.5, i++) {
		if (_angle < a && _angle >= (a - 22.5)) {
			angleSprite = (int)i;
		}
		if (i == 7.0) i = -1.0;
	}
	this->timeAlive = 250;
	this->decaySprite = 0;
	this->image = &assets->images.BoostSprite[this->angleSprite];
	this->viewportRect = Graphics::CreateRect(32, 32, 0, 0);
}

void BoostStreak::UpdateDecaySprite(Uint32 timestep) {
	this->timeAlive -= timestep;
	if (this->timeAlive > 100) this->decaySprite = 0;
	else if (this->timeAlive > 50) this->decaySprite = 1;
	else if (this->timeAlive > 0) this->decaySprite = 2;
	if (this->timeAlive < 0) this->timeAlive = 0;
}
