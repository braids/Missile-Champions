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
			// Start game event
			if (Event->key.keysym.sym == SDLK_SPACE && Event->key.repeat == 0) {
				Mix_HaltMusic();
				Mix_PlayChannel(CHANNEL_TITLESTART, mAssets->sounds.StartSelection, 0);
				sceneTitleScreen.events.StartGame = true;
			}
			// Exit game event
			if (Event->key.keysym.sym == SDLK_ESCAPE)
				Running = false;
			break;

		case Scene_CarSelection:
			// Cursor control events
			if (Event->key.keysym.sym == SDLK_UP && Event->key.repeat == 0) {
				Mix_PlayChannel(CHANNEL_CURSOR, mAssets->sounds.MoveCursor, 0);
				CarSelectionCursor.SelectEvent |= CarSelectionCursor.SelectUp;
			}
			if (Event->key.keysym.sym == SDLK_DOWN && Event->key.repeat == 0) {
				Mix_PlayChannel(CHANNEL_CURSOR, mAssets->sounds.MoveCursor, 0);
				CarSelectionCursor.SelectEvent |= CarSelectionCursor.SelectDown;
			}
			if (Event->key.keysym.sym == SDLK_LEFT && Event->key.repeat == 0) {
				Mix_PlayChannel(CHANNEL_CURSOR, mAssets->sounds.MoveCursor, 0);
				CarSelectionCursor.SelectEvent |= CarSelectionCursor.SelectLeft;
			}
			if (Event->key.keysym.sym == SDLK_RIGHT && Event->key.repeat == 0) {
				Mix_PlayChannel(CHANNEL_CURSOR, mAssets->sounds.MoveCursor, 0);
				CarSelectionCursor.SelectEvent |= CarSelectionCursor.SelectRight;
			}
			// Car selection event
			if (Event->key.keysym.sym == SDLK_SPACE && Event->key.repeat == 0 && !Event_P2Selected) {
				Mix_PlayChannel(CHANNEL_SELECTION, mAssets->sounds.Selection, 0);
				Event_CarSelected = true;
			}
			// Exit to title screen
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				Mix_HaltMusic();
				CurrentScene = Scene_TitleScreen;
			}
			break;

		case Scene_Gameplay:
			// Active car control events
			if (Event->key.keysym.sym == SDLK_UP)
				Players[0].activeCar->MoveDirection = Car::Forward;
			if (Event->key.keysym.sym == SDLK_DOWN)
				Players[0].activeCar->MoveDirection = Car::Backward;
			if (Event->key.keysym.sym == SDLK_LEFT)
				Players[0].activeCar->Turning = Car::Left;
			if (Event->key.keysym.sym == SDLK_RIGHT)
				Players[0].activeCar->Turning = Car::Right;
			// Change active car event
			if (Event->key.keysym.sym == SDLK_x && Event->key.repeat == 0)
				Event_ChangeCar = true;
			if (Event->key.keysym.sym == SDLK_z)
				Players[0].activeCar->isBoosting = true;
			if (Event->key.keysym.sym == SDLK_SPACE && Players[0].activeCar->z <= 0 && Event->key.repeat == 0)
				Players[0].activeCar->isJumping = true;
			// Exit to title screen
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				Mix_VolumeMusic(MIX_MAX_VOLUME);
				Mix_HaltMusic();
				RoundTimer.stop();
				CurrentScene = Scene_TitleScreen;
			}
			break;
			
		case Scene_GameOver:
			// Exit to title screen
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				Mix_HaltMusic();
				GameOverTimer.stop();
				CurrentScene = Scene_TitleScreen;
			}
			break;

		case Scene_Credits:
			// Exit to title screen
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				Mix_HaltMusic();
				CreditsTimer.stop();
				CurrentScene = Scene_TitleScreen;
			}
			break;
		}
		break;
	case SDL_KEYUP:
		switch (CurrentScene) {
		case Scene_Gameplay:
			// Active car control reset events
			if (Event->key.keysym.sym == SDLK_UP && Players[0].activeCar->MoveDirection != Car::Backward)
				Players[0].activeCar->MoveDirection = Car::NoMovement;
			if (Event->key.keysym.sym == SDLK_DOWN && Players[0].activeCar->MoveDirection != Car::Forward)
				Players[0].activeCar->MoveDirection = Car::NoMovement;
			if (Event->key.keysym.sym == SDLK_LEFT && Players[0].activeCar->Turning != Car::Right)
				Players[0].activeCar->Turning = Car::NoTurning;
			if (Event->key.keysym.sym == SDLK_RIGHT && Players[0].activeCar->Turning != Car::Left)
				Players[0].activeCar->Turning = Car::NoTurning;
			if (Event->key.keysym.sym == SDLK_z)
				Players[0].activeCar->isBoosting = false;
		}
		break;
	default:
		break;
	}
}