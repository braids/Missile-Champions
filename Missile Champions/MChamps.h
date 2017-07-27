#ifndef _MCHAMPS_H_
#define _MCHAMPS_H_

# ifndef _MSC_VER
# define SDL_MAIN_HANDLED
# endif

#include "Objects.h"

#define CAMERA_W 256
#define CAMERA_H 176
#define MAX_BOOST_FUEL 10000
#define MIN_BOOST_FUEL 0

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
	Assets::Image*	BoostBar;
	SDL_Rect*		BoostBarScaleRect;

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
