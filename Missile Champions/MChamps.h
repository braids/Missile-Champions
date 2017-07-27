#ifndef _MCHAMPS_H_
#define _MCHAMPS_H_

# ifndef _MSC_VER
# define SDL_MAIN_HANDLED
# endif

#include "Graphics.h"
#include "Assets.h"
#include "Timer.h"

#define CAMERA_W 256
#define CAMERA_H 176

class MChamps {
private:
	// Running Flag
	bool            Running;

	// Timer Vars
	Uint32			currTick;
	Uint32			lastTick;
	Uint32			timeStep;

	// Scenes
	enum Scenes { 
		Scene_TitleScreen, 
		Scene_CarSelection, 
		Scene_Gameplay 
	} CurrentScene;

	// Graphics
	Graphics*		mGraphics;

	// Assets
	Assets*			mAssets;

	//// Object structs
	// Boost Streak
	struct BoostStreak {
		Assets::Image* image;
		SDL_Rect* viewportRect;
		int angleSprite, decaySprite;
		double x, y, z;
		int timeAlive;

		void SpawnSprite(double _x, double _y, double _angle, Assets* assets) {
			this->x = _x;
			this->y = _y;
			for (double a = 11.25, i = 0; a <= 371.25; a += 22.5, i++) {
				if (_angle < a && _angle >= (a - 22.5)) {
					angleSprite = i;
				}
				if (i == 7) i = -1;
			}
			this->timeAlive = 250;
			this->decaySprite = 0;
			this->image = &assets->images.BoostSprite[this->angleSprite];
			this->viewportRect = Graphics::CreateRect(32, 32, 0, 0);
		}

		void UpdateDecaySprite(Uint32 timestep) {
			timeAlive -= timestep;
			if (timeAlive > 100) decaySprite = 0;
			else if (timeAlive > 50) decaySprite = 1;
			else if (timeAlive > 0) decaySprite = 2;
			if (timeAlive < 0) timeAlive = 0;
		}
	};
	// Car
	struct Car {
		Assets::Image* image;
		SDL_Rect* viewportRect;
		int anglesprite;
		double x, y, z;
		double vx, vy, vz;
		double dx, dy;
		double angle, speed;
		bool ballCollide;
		bool isBoosting;
		int boostStreakCounter;
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
		BoostStreak streak[5];
		double cx() { return x + (double) (viewportRect->w / 2); }
		double cy() { return y + (double) (viewportRect->h / 2); }
	};
	
	// Player
	struct Player {
		int		team;
		int		score;
		SDL_Rect* selectionViewport;
		Car*	activeCar;
		Car		cars[3];
	} Players[2];
	int ActiveCar;

	// Ball
	struct Ball {
		Assets::Image* image;
		SDL_Rect* viewportRect;
		int frame;
		double x, y, z;
		double dx, dy, dz;
		double vx, vy;
		double speed;
		Timer ballAnimate;
		void updateSpeed(Uint32 ts);
		double cx() { return x + (double) (viewportRect->w / 2); }
		double cy() { return y + (double) (viewportRect->h / 2); }
	} GameBall;

	// Camera
	struct Camera {
		Assets::Image* drawarea;
		Assets::Image* viewport;
		float x, y;
		float dx, dy;
	} GameplayCamera;

	// Car Selection Cursor
	struct Cursor {
		Assets::Image* image;
		int column, row;
		enum Event {
			SelectUp = 1 << 0,
			SelectDown = 1 << 1,
			SelectLeft = 1 << 2,
			SelectRight = 1 << 3,
			NoSelection = 0
		} SelectEvent;
		friend Event operator |(Event a, Event b) {	return (Event) ((int) a | (int) b); }
		friend Event& operator |=(Event& a, Event b) { return a = a | b; }
		friend Event operator &(Event a, Event b) { return (Event) ((int) a & (int) b); }
		friend Event& operator &=(Event& a, Event b) { return a = a & b; }
	} CarSelectionCursor;

	// Images
	Assets::Image*	TitleScreenBG;
	Assets::Image*	CarSelectBG;
	Assets::Image*	StatusBar;
	Assets::Image*	FieldBottom;

	// Events and effects
	bool Event_CarSelected;
	Uint32 Effect_P1FlashLength;
	bool Event_P1Selected;
	Uint32 Effect_P2FlashLength;
	bool Event_P2Selected;
	Uint32 Effect_StartFlashLength;
	bool Event_StartGame;
	bool Event_ChangeCar;

public:
	MChamps();

	int OnExecute();

	bool OnInit();

	void OnEvent(SDL_Event* Event);

	void OnLoop();
		void BallUpdate();
		void PlayerCarsUpdate(Player * player);

	void OnRender();
		void DrawImage(Assets::Image * image);
		void DrawImage(Assets::Image * image, SDL_Rect * drawRect);
		void DrawImageFrame(Assets::Image * image, SDL_Rect * drawRect, int frame);

	void OnCleanup();
};

#endif
