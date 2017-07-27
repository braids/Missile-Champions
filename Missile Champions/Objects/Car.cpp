#include "../Objects.h"

void Car::InitCar() {
	// Image
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

double Car::cx() { 
	return this->x + (double)this->viewportRect->w / 2.0; 
}

double Car::cy() {
	return this->y + (double)this->viewportRect->y / 2.0;
}
