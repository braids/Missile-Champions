#ifndef _CAR_H_
#define _CAR_H_

#include "Assets.h"
#include "Timer.h"

#define MAX_BOOST_FUEL 10000
#define MIN_BOOST_FUEL 0

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
	Assets* assets;
	Assets::Image* image;
	SDL_Rect* viewportRect;
	int anglesprite;
	double x, y, z;
	double dx, dy, dz;
	double angle, speed;
	bool ballCollide;
	bool isBoosting;
	bool isJumping;
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
	Timer jumpTimer;
	BoostStreak streak[5];
	
	void InitCar(Assets* a);
	void SetCarSelect(int team);
	void SetCarKickoff(int team, int car);
	double cx();
	double cy();
	double cz();
};

struct Ball {
	Assets::Image* sprites;
	Assets::Image* image;
	SDL_Rect* viewportRect;
	int frame;
	double x, y, z;
	double dx, dy, dz;
	double speed;
	Timer ballAnimate;
	void updatePosition(Uint32 ts);
	void initBall(Assets::Image* a);
	void resetBall();
	void UpdateViewport(SDL_Rect* camera);
	double cx();
	double cy();
	double cz();
};

struct Player {
	int		team;
	int		score;
	SDL_Rect* selectionViewport;
	Car*	activeCar;
	Car		cars[3];
};

#endif
