#ifndef _CAR_H_
#define _CAR_H_

#include "Assets.h"
#include "Timer.h"

#define MAX_BOOST_FUEL 10000
#define MIN_BOOST_FUEL 0

// Forward decs
struct Ball;
struct BoostStreak;
struct Car;
struct Player;
struct Camera;

struct BoostStreak {
	Car* parent;
	Assets::Image* image;
	SDL_Rect* viewportRect;
	int angleSprite, decaySprite;
	double x, y, z;
	int timeAlive;

	void SpawnSprite(Assets* assets, Car* car);
	void UpdateDecaySprite(Uint32 timestep);
	void UpdateViewport(Camera* camera);
};

struct Car {
	Player* parent;
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
	
	void InitCar(Assets* a, Player* player);
	void SetCarSelect();
	void SetCarKickoff(int car);
	void UpdateViewport(Camera* camera);
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
	void UpdateViewport(Camera* camera);
	double cx();
	double cy();
	double cz();
};

struct Player {
	int		player_num;
	int		team;
	int		score;
	SDL_Rect* selectionViewport;
	Car*	activeCar;
	Car		cars[3];
	void Init(Assets* assets, int p_num);
	void SetCarSelection();
	void SetStartRound();
	void SetKickoff();
};

struct Camera {
	Assets::Image* drawarea;
	Assets::Image* viewport;
	double x, y;
	double dx, dy;
	void Init(Assets::Image* fielddraw, Assets::Image* fieldview);
	void CenterOnCar(Car* activeCar);
};

#endif
