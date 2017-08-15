#ifndef _MCHAMPS_H_
#define _MCHAMPS_H_

# ifndef _MSC_VER
# define SDL_MAIN_HANDLED
# endif

#include <random>
#include "Objects.h"

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

	// Random generator
	std::default_random_engine randomEngine;

	// Scenes
	enum Scenes { 
		Scene_TitleScreen, 
		Scene_CarSelection, 
		Scene_Gameplay,
		Scene_Credits,
		Scene_GameOver
	} CurrentScene;

	SceneManager scene;

	// Graphics
	Graphics*		mGraphics;

	// Assets
	Assets*			mAssets;

	//// Object structs
	// Player
	Player Players[2];
	int ActiveCar;

	Ball GameBall;

	// Camera
	Camera GameplayCamera;

	// Car Selection Cursor
	Cursor CarSelectionCursor;

	// Images
	Assets::Image*	CarSelectBG;
	Assets::Image*	StatusBar;
	Assets::Image*	FieldBottom;

	SDL_Rect* BallIndicatorRect;
	bool BallOffscreen;
	int BallIndicatorDirection;

	SDL_Rect* P1Score;
	SDL_Rect* P2Score;
	SDL_Rect* Minute10sRect;
	SDL_Rect* Minute1sRect;
	SDL_Rect* Second10sRect;
	SDL_Rect* Second1sRect;
	Assets::Image* Countdown321;
	SDL_Rect* Countdown321Rect;
	Assets::Image* CountdownG;
	SDL_Rect* CountdownGRect;
	Assets::Image* CountdownO;
	SDL_Rect* CountdownORect;
	SDL_Rect* CreditsRect;
	double CreditsY;

	Assets::Image*	BoostBar;
	SDL_Rect*		BoostBarScaleRect;
	Car* drawCars[6];

	Timer	MusicTimer;
	Timer	ShadowBlinkTimer;
	Timer	RoundStartTimer;
	Timer	RoundTimer;
	Uint32	RoundTicks;
	Timer	GoalTimer;
	Timer	CreditsTimer;
	Uint32	creditsTicks;
	Timer	GameOverTimer;

	// Events and effects
	bool Event_CarSelected;
	Uint32 Effect_P1FlashLength;
	bool Event_P1Selected;
	Uint32 Effect_P2FlashLength;
	bool Event_P2Selected;
	bool Event_ChangeCar;
	bool Event_LeftGoal;
	bool Event_RightGoal;

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
		void DrawBall(int shadowticks);

	void OnCleanup();
};

#endif
