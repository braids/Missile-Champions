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
	float			timeStep;

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
	// Car
	struct Car {
		Assets::Image* image;
		SDL_Rect* viewportRect;
		int anglesprite;
		double x, y;
		double vx, vy;
		double dx, dy;
		double angle, speed;
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
		void PlayerCarsUpdate(Player * player);

	void OnRender();
		void DrawImage(Assets::Image * image);
		void DrawImage(Assets::Image * image, SDL_Rect * drawRect);

	void OnCleanup();
};

#endif
