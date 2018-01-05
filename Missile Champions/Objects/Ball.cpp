#include "../Objects.h"

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

void Ball::CarCollision(Player* player) {
	// Ball Collision
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			// Check sphere collision between car and ball if not currently colliding
			if (sqrt(
				pow(this->cx() - player[i].cars[j].cx(), 2) +
				pow(this->cy() - player[i].cars[j].cy(), 2) +
				pow(this->cz() - player[i].cars[j].cz(), 2)) <= 40.0 &&
				player[i].cars[j].ballCollide == false) {
				// Play ballhit sound
				//Mix_PlayChannel(CHANNEL_BALLHIT, this->SoundBallhit, 0);

				// Set colliding flag
				player[i].cars[j].ballCollide = true;

				// Get collision angle in rads (atan2), convert to deg (* 180 / M_PI)
				double newAngle = atan2(this->cy() - player[i].cars[j].cy(),
					this->cx() - player[i].cars[j].cx()) * 180.0 / M_PI;
				double newZX = atan2(this->cx() - player[i].cars[j].cx(),
					this->cz() - player[i].cars[j].cz()) * 180.0 / M_PI;

				// Rotate to match axes
				newAngle -= 90.0;
				newZX += 90.0;

				// Keep angle between 0 - 359 deg inclusive
				if (newAngle >= 360.0) newAngle -= 360.0;
				if (newAngle < 0.0) newAngle += 360.0;
				// Invert angle
				newAngle = 360.0 - newAngle;

				// Keep angle between 0 - 359 deg inclusive
				if (newZX >= 360.0) newZX -= 360.0;
				if (newZX < 0.0) newZX += 360.0;
				// Invert angle
				newZX = 360.0 - newZX;

				// Debug output of ball collision angles
				//std::cout << "Ball collision angle: " << newAngle << "\nsin(newAngle): " << sin(newAngle) << "\ncos(newAngle): " << cos(newAngle) << std::endl;
				//std::cout << "Ball cx: " << this->cx() << "\nBall cy: " << this->cy() << "\nBall cz: " << this->cz() << std::endl;
				//std::cout << "Car cx: " << player[i].cars[j].cx() << "\nCar cy: " << player[i].cars[j].cy() << "\nCar cz: " << player[i].cars[j].cz() << std::endl;

				// Ball direction set to collision angle
				this->dx = sin(newAngle * M_PI / 180.0);
				this->dy = cos(newAngle * M_PI / 180.0);

				// If car colliding is faster on x/y/z, get new dz value
				if (abs(player[i].cars[j].speed * 1.25) > this->speed ||
					abs(player[i].cars[j].speed * 1.25) > abs(this->dx))
					this->dz = abs(sin(newZX * M_PI / 180.0)) + abs(cos(newZX * M_PI / 180.0));
				//std::cout << "Ball dz: " << this->dz << std::endl;

				// Car speed added to ball speed
				// If car colliding is faster, add to ball speed.
				if (abs(player[i].cars[j].speed * 1.25) > this->speed)
					this->speed += abs(player[i].cars[j].speed * 1.25);
			}
			// If car/ball spheres no longer colliding, set collision flag false
			else if (sqrt(
				pow(this->cx() - player[i].cars[j].cx(), 2) +
				pow(this->cy() - player[i].cars[j].cy(), 2) +
				pow(this->cz() - player[i].cars[j].cz(), 2)) > 40.0) {
				player[i].cars[j].ballCollide = false;
			}
		}
	}
}

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

void Ball::UpdateSprite() {
	// Ball Sprite Update
	Uint32 ballAnimTicks = this->ballAnimate.getTicks();
	Uint32 ballAnimSpeed = 1000 - (Uint32)(1000.0 * this->speed * 1.5); // low: -0, high: -350
	if (this->ballAnimate.isStarted()) {
		if (this->dx < 0.0 && this->speed > 0.0) {
			if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.25)) this->frame = 3;
			else if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.5)) this->frame = 2;
			else if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.75)) this->frame = 1;
			else this->frame = 0;
		}
		if (this->dx >= 0.0 && this->speed > 0.0) {
			if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.25)) this->frame = 1;
			else if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.5)) this->frame = 2;
			else if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.75)) this->frame = 3;
			else this->frame = 0;
		}
		if (ballAnimTicks > ballAnimSpeed) {
			this->ballAnimate.stop();
			this->ballAnimate.start();
		}
	}
}

void Ball::UpdateViewport(Camera* camera) {
	this->viewportRect->x = (int)this->x - camera->drawarea->rect->x;
	this->viewportRect->y = (int)(this->y - this->z) - camera->drawarea->rect->y;
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