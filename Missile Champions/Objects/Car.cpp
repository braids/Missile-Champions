#include "../Objects.h"

void Car::InitCar(Assets* a) {
	// Image
	this->assets = a;
	this->image = nullptr;
	this->viewportRect = Graphics::CreateRect(32, 32, 0, 0);

	// Car physics/positioning attributes
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
	this->dx = 0.0;
	this->dy = 0.0;
	this->dz = 0.0;
	this->angle = 0.0;
	this->speed = 0.0;
	this->anglesprite = 0;
	
	// Car direction
	this->MoveDirection = Car::Forward;
	this->Turning = Car::Left;
	
	// Car boost
	this->boostStreakCounter = 0;
	this->boostFuel = 0;
	this->ballCollide = false;
	this->isBoosting = false;
	this->boostStreakTimer.stop();
	this->boostRechargeTimer.stop();
}

void Car::SetCarSelect(int team) {
	this->image = nullptr;				// Set image null (no displaying on screen)
	
	if (team == 0) {
		this->viewportRect->x = 56;		// Player 1 Xpos
		this->anglesprite = 4;			// Player 1 angle sprite
	}
	else {
		this->viewportRect->x = 168;	// Player 2 Xpos
		this->anglesprite = 12;			// Player 2 angle sprite
	}
	this->viewportRect->y = 24;			// Player 1/2 Ypos
}

void Car::SetCarKickoff(int team, int carpos) {
	switch (team) {
	case 0:
		switch (carpos) {
		case 0:
			this->x = 400.0;
			this->y = 192.0;
			break;
		case 1:
			this->x = 368.0;
			this->y = 160.0;
			break;
		case 2:
			this->x = 368.0;
			this->y = 224.0;
			break;
		}
		this->angle = 90.0;
		this->anglesprite = 4;
		break;
	case 1:
		switch (carpos) {
		case 0:
			this->x = 592.0;
			this->y = 192.0;
			break;
		case 1:
			this->x = 624.0;
			this->y = 160.0;
			break;
		case 2:
			this->x = 624.0;
			this->y = 224.0;
			break;
		}
		this->angle = 270.0;
		this->anglesprite = 12;
		break;
	}
	this->image = &this->assets->images.CarSprites[this->anglesprite][team];
	this->dx = sin(this->angle * M_PI / 180.0);
	this->dy = cos(this->angle * M_PI / 180.0);
	this->speed = 0.0;
	this->boostStreakCounter = 0;
	this->boostFuel = MAX_BOOST_FUEL;
	this->MoveDirection = Car::NoMovement;
	this->Turning = Car::NoTurning;
}

double Car::cx() { 
	return this->x + (double)this->viewportRect->w / 2.0; 
}

double Car::cy() {
	return this->y + (double)this->viewportRect->h / 2.0;
}
