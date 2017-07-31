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
		Scene_Gameplay 
	} CurrentScene;

	// Graphics
	Graphics*		mGraphics;

	// Assets
	Assets*			mAssets;

	//// Object structs
	// Player
	struct Player {
		int		team;
		int		score;
		SDL_Rect* selectionViewport;
		Car*	activeCar;
		Car		cars[3];
	} Players[2];
	int ActiveCar;

	Ball GameBall;

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
	SDL_Rect* P1Score;
	SDL_Rect* P2Score;
	SDL_Rect* Minute10sRect;
	SDL_Rect* Minute1sRect;
	SDL_Rect* Second10sRect;
	SDL_Rect* Second1sRect;
	Assets::Image*	BoostBar;
	SDL_Rect*		BoostBarScaleRect;
	Car* drawCars[6];

	Timer	TitleMusicTimer;
	Timer	ShadowBlinkTimer;
	Timer	RoundTimer;

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
		void DrawBall(int shadowticks);

	void OnCleanup();
};

#endif
