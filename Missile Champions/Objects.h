#ifndef _CAR_H_
#define _CAR_H_

#include "Assets.h"
#include "Timer.h"

// Boost defs
#define MAX_BOOST_FUEL 10000
#define MIN_BOOST_FUEL 0

// Cursor defs
#define CURSOR_P1_DEFAULT_X 16
#define CURSOR_P1_DEFAULT_Y 64
#define CURSOR_P2_DEFAULT_X 128
#define CURSOR_P2_DEFAULT_Y 64
#define CURSOR_MOVE_X 48
#define CURSOR_MOVE_Y 32
#define CURSOR_ROW_MIN 0
#define CURSOR_ROW_MAX 3
#define CURSOR_COL_MIN 0
#define CURSOR_COL_MAX 1

// Forward decs
struct Ball;
struct BoostStreak;
struct Car;
struct Player;
struct Camera;
struct Cursor;

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

struct Cursor {
	Assets::Image* image;
	int column, row;
	int defaultX, defaultY;
	enum Event {
		SelectUp = 1 << 0,
		SelectDown = 1 << 1,
		SelectLeft = 1 << 2,
		SelectRight = 1 << 3,
		NoSelection = 0
	} SelectEvent;
	friend Event operator |(Event a, Event b) { return (Event)((int)a | (int)b); }
	friend Event& operator |=(Event& a, Event b) { return a = a | b; }
	friend Event operator &(Event a, Event b) { return (Event)((int)a & (int)b); }
	friend Event& operator &=(Event& a, Event b) { return a = a & b; }

	void Init(Assets::Image* cursor);
	void SetP1();
	void SetP2();
	void MoveCursor();
	int GetSelection();
};

#endif
