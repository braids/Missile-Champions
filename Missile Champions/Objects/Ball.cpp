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
		this->speed = 0.0;
	}
}