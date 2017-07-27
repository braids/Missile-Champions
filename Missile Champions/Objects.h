#ifndef _CAR_H_
#define _CAR_H_

#include "Assets.h"
#include "Timer.h"

struct BoostStreak {
	Assets::Image* image;
	SDL_Rect* viewportRect;
	int angleSprite, decaySprite;
	double x, y, z;
	int timeAlive;

	void SpawnSprite(double _x, double _y, double _angle, Assets* assets);
	void UpdateDecaySprite(Uint32 timestep);
};

struct Car {
	Assets::Image* image;
	SDL_Rect* viewportRect;
	int anglesprite;
	double x, y, z;
	double dx, dy;// , dz;
	double angle, speed;
	bool ballCollide;
	bool isBoosting;
	int boostStreakCounter;
	int boostFuel;
	enum Movement {
		NoMovement,
		Forward,
		Backward
	} MoveDirection;

	enum Turn {
		NoTurning,
		Left,
		Right
	} Turning;
	Timer boostStreakTimer;
	Timer boostRechargeTimer;
	BoostStreak streak[5];
	double dz;
	void InitCar();

	double cx();
	double cy();
};



#endif
