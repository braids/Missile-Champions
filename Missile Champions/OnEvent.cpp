#include "MChamps.h"

void MChamps::OnEvent(SDL_Event* Event) {
	// Handle general events
	switch (Event->type) {
	case SDL_QUIT:
		Running = false;
		break;
	case SDL_KEYDOWN:
		switch (CurrentScene) {
		case Scene_TitleScreen:
			if (Event->key.keysym.sym == SDLK_SPACE)
				Event_StartGame = true;
			if (Event->key.keysym.sym == SDLK_ESCAPE)
				Running = false;
			break;

		case Scene_CarSelection:
			if (Event->key.keysym.sym == SDLK_UP && Event->key.repeat == 0)
				CarSelectionCursor.SelectEvent |= CarSelectionCursor.SelectUp;
			if (Event->key.keysym.sym == SDLK_DOWN && Event->key.repeat == 0)
				CarSelectionCursor.SelectEvent |= CarSelectionCursor.SelectDown;
			if (Event->key.keysym.sym == SDLK_LEFT && Event->key.repeat == 0)
				CarSelectionCursor.SelectEvent |= CarSelectionCursor.SelectLeft;
			if (Event->key.keysym.sym == SDLK_RIGHT && Event->key.repeat == 0)
				CarSelectionCursor.SelectEvent |= CarSelectionCursor.SelectRight;
			if (Event->key.keysym.sym == SDLK_SPACE && Event->key.repeat == 0)
				Event_CarSelected = true;
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				CurrentScene = Scene_TitleScreen;
			}
			break;

		case Scene_Gameplay:
			if (Event->key.keysym.sym == SDLK_UP)
				Players[0].activeCar->MoveDirection = Car::Forward;
			if (Event->key.keysym.sym == SDLK_DOWN)
				Players[0].activeCar->MoveDirection = Car::Backward;
			if (Event->key.keysym.sym == SDLK_LEFT)
				Players[0].activeCar->Turning = Car::Left;
			if (Event->key.keysym.sym == SDLK_RIGHT)
				Players[0].activeCar->Turning = Car::Right;
			if (Event->key.keysym.sym == SDLK_c && Event->key.repeat == 0)
				Event_ChangeCar = true;
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				CurrentScene = Scene_TitleScreen;
			}
			break;			
		}
		break;
	case SDL_KEYUP:
			if (Event->key.keysym.sym == SDLK_UP && Players[0].activeCar->MoveDirection != Car::Backward)
				Players[0].activeCar->MoveDirection = Car::NoMovement;
			if (Event->key.keysym.sym == SDLK_DOWN && Players[0].activeCar->MoveDirection != Car::Forward)
				Players[0].activeCar->MoveDirection = Car::NoMovement;
			if (Event->key.keysym.sym == SDLK_LEFT && Players[0].activeCar->Turning != Car::Right)
				Players[0].activeCar->Turning = Car::NoTurning;
			if (Event->key.keysym.sym == SDLK_RIGHT && Players[0].activeCar->Turning != Car::Left)
				Players[0].activeCar->Turning = Car::NoTurning;
		break;
	default:
		break;
	}

	// Game object event handling
	// GameObject->HandleEvent(Event);
}