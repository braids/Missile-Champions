#include "../Objects.h"

void Ball::updatePosition(Uint32 ts) {
	////// XY Speed
	// If speed too high, lower to speed cap
	if (this->speed > 0.35) {
		this->speed = 0.35;
	}
	
	// If there is speed
	if (this->speed > 0.0) {
		// Start animating ball if it isn't started
		if (!this->ballAnimate.isStarted()) 
			this->ballAnimate.start();
		
		//Reduce speed	
		this->speed -= 0.0001 * (double)ts;
	}

	// If no speed
	if (this->speed <= 0.0) {
		// Stop ball animation
		if (this->ballAnimate.isStarted())
			this->ballAnimate.stop();

		// Stop all movement, return speed to 0
		this->dx = 0.0;
		this->dy = 0.0;
		this->speed = 0.0;
	}

	////// Z Speed
	//// On ground/bounce
	// If z direction down and on ground
	if (this->dz < 0.0 && this->z <= 0.0) {
		// If z direction speed low enough, stop z movement
		if (this->dz > -0.1)
			this->dz = 0;

		// Reverse z direction at half speed
		this->dz *= -0.5;
	}

	//// In Air
	// If in air, reduce direction downward
	if (this->z > 0.0)
		this->dz -= 0.0033 * (double)ts;

	////// Move ball
	this->x += this->dx * this->speed * (double)ts;
	this->y += this->dy * this->speed * (double)ts;
	this->z += this->dz * (double)ts * 0.2;

	////// Position Correction
	//// Z Position
	// If below floor, raise to floor.
	if (this->z <= 0.0) {
		this->z = 0.0;
	}
	// If above ceiling, lower to ceiling and reverse z direction.
	if (this->z >= 96.0) {
		this->z = 96.0;
		this->dz *= -1.0;
	}

	//// Wall collision
	double moveBallX = 0.0;
	double moveBallY = 0.0;

	// Inner wall collision
	if (this->x < 32.0 && (this->cy() < 116.0 || this->cy() > 236.0 + 48.0)) {
		moveBallX = 32.0;
	}
	if (this->x > 1024.0 - 32.0 - 48.0 && (this->cy() < 116.0 || this->cy() > 236.0 + 48.0)) {
		moveBallX = 1024.0 - 32.0 - 48.0;
	}
	if (this->y < 116.0 && (this->cx() < 32.0 || this->cx() > 1024.0 - 32.0)) {
		moveBallY = 116.0;
	}
	if (this->y > 236.0 && (this->cx() < 32.0 || this->cx() > 1024.0 - 32.0)) {
		moveBallY = 236.0;
	}
	if (moveBallX > 0.0) { this->dx *= -1.0; this->x = moveBallX; }
	if (moveBallY > 0.0) { this->dy *= -1.0; this->y = moveBallY; }

	// Outer boundary collision
	if (this->x < -24.0) {
		this->x = -24.0;
		this->dx *= -1.0;
	}
	if (this->x > 1000.0) {
		this->x = 1000.0;
		this->dx *= -1.0;
	}
	if (this->y < 20.0) {
		this->y = 20.0;
		this->dy *= -1.0;
	}
	if (this->y > 348.0) {
		this->y = 348.0;
		this->dy *= -1.0;
	}
}

void Ball::initBall(Assets::Image* a) {
	this->sprites = a;
	this->frame = 0;
	this->image = &this->sprites[this->frame];
	this->viewportRect = Graphics::CreateRect(48, 48, 0, 0);
	this->x = 0.0;
	this->y = 0.0;
	this->z = 0.0;
	this->dx = 0.0;
	this->dy = 0.0;
	this->dz = 0.0;
	this->speed = 0.0;
}

void Ball::resetBall() {
	this->x = 512.0 - ((double)this->viewportRect->w / 2.0);
	this->y = 200.0 - ((double)this->viewportRect->h / 2.0);
	this->z = 20.0;
	this->dx = 0.0;
	this->dy = 0.0;
	this->dz = 0.0;
	this->speed = 0.0;
	this->viewportRect->x = 0;
	this->viewportRect->y = 0;
}

void Ball::UpdateViewport(SDL_Rect* camera) {
	this->viewportRect->x = (int)this->x - camera->x;
	this->viewportRect->y = (int)(this->y - this->z) - camera->y;
}

double Ball::cx() { 
	return this->x + ((double)this->viewportRect->w / 2.0);
}
double Ball::cy() { 
	return this->y + ((double)this->viewportRect->h / 2.0);
}
double Ball::cz() {
	return this->z + ((double)this->viewportRect->h / 2.0);
}