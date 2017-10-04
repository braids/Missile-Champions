#include "../Objects.h"

void Camera::Init(Assets::Image* fielddraw, Assets::Image* fieldview) { 
	this->drawarea = fielddraw;
	this->viewport = fieldview; 
	this->x = 0.0;
	this->y = 0.0;
	this->dx = 0.0;
	this->dy = 0.0;
}

void Camera::CenterOnCar(Car* activeCar) {
	// Center on car x
	this->drawarea->rect->x = ((int)activeCar->x + activeCar->viewportRect->w / 2) - this->drawarea->rect->w / 2;
	// Center on car y
	this->drawarea->rect->y = ((int)activeCar->y + activeCar->viewportRect->h / 2) - this->drawarea->rect->h / 2;
	// Prevent camera from leaving x boundary
	if (this->drawarea->rect->x < 0) this->drawarea->rect->x = 0;
	if (this->drawarea->rect->x > 768) this->drawarea->rect->x = 768;
	// Prevent camera from leaving y boundary
	if (this->drawarea->rect->y < 0) this->drawarea->rect->y = 0;
	if (this->drawarea->rect->y > 208) this->drawarea->rect->y = 208;
}
