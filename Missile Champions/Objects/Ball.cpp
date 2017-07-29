#include "../Objects.h"

void Ball::updateSpeed(Uint32 ts) {
	if (this->speed > 0.4) {
		this->speed = 0.4;
	}
	if (this->speed > 0.0) {
		if (!this->ballAnimate.isStarted()) {
			this->ballAnimate.start();
		}
		this->speed -= 0.000225 * (double)ts;
	}
	if (this->speed <= 0.0) {
		if (this->ballAnimate.isStarted()) {
			this->ballAnimate.stop();
		}
		this->dx = 0.0;
		this->dy = 0.0;
		this->dz = 0.0;
		this->speed = 0.0;
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
	this->y = 208.0 - ((double)this->viewportRect->h / 2.0);
	this->z = 0.0;
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