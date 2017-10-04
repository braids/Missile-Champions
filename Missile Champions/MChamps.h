#ifndef _MCHAMPS_H_
#define _MCHAMPS_H_

# ifndef _MSC_VER
# define SDL_MAIN_HANDLED
# endif

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

	// Scenes
	Scenes CurrentScene;

	SceneManager scene;

	// Graphics
	Graphics*		mGraphics;

	// Assets
	Assets*			mAssets;

	//// Object structs
	// Player
	Player Players[2];
	Car* drawCars[6];

public:
	MChamps();

	int OnExecute();

	bool OnInit();

	void OnEvent(SDL_Event* Event);

	void OnLoop();

	void OnRender();
		void DrawImage(Assets::Image * image);
		void DrawImage(Assets::Image * image, SDL_Rect * drawRect);
		void DrawBall(int shadowticks);

	void OnCleanup();
};

#endif
