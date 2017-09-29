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
				scene.titleScreen.events.StartGame = true;
			}
			// Exit game event
			if (Event->key.keysym.sym == SDLK_ESCAPE)
				Running = false;
			break;

		case Scene_CarSelection:
			// Cursor control events
			if (Event->key.keysym.sym == SDLK_UP && Event->key.repeat == 0) {
				scene.carSelection.SelectCursor.SelectEvent |= scene.carSelection.SelectCursor.SelectUp;
			}
			if (Event->key.keysym.sym == SDLK_DOWN && Event->key.repeat == 0) {
				scene.carSelection.SelectCursor.SelectEvent |= scene.carSelection.SelectCursor.SelectDown;
			}
			if (Event->key.keysym.sym == SDLK_LEFT && Event->key.repeat == 0) {
				scene.carSelection.SelectCursor.SelectEvent |= scene.carSelection.SelectCursor.SelectLeft;
			}
			if (Event->key.keysym.sym == SDLK_RIGHT && Event->key.repeat == 0) {
				scene.carSelection.SelectCursor.SelectEvent |= scene.carSelection.SelectCursor.SelectRight;
			}
			// Car selection event
			if (Event->key.keysym.sym == SDLK_SPACE && Event->key.repeat == 0 && !scene.carSelection.events.SelectP2) {
				Mix_PlayChannel(CHANNEL_SELECTION, scene.carSelection.SoundSelection, 0);
				scene.carSelection.events.Select = true;
			}
			// Exit to title screen
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				Mix_HaltMusic();
				CurrentScene = Scene_TitleScreen;
				scene.StartScene(Scene_TitleScreen);
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
			// Cheats
			if (Event->key.keysym.sym == SDLK_s && Event->key.repeat == 0)
				Players[0].score = 8;
			if (Event->key.keysym.sym == SDLK_d && Event->key.repeat == 0)
				Players[1].score = 8;
			// Exit to title screen
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				Mix_VolumeMusic(MIX_MAX_VOLUME);
				Mix_HaltMusic();
				RoundTimer.stop();
				CurrentScene = Scene_TitleScreen;
				scene.StartScene(Scene_TitleScreen);
			}
			break;
			
		case Scene_GameOver:
			// Exit to title screen
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				scene.gameOver.WaitTimer.stop();
				CurrentScene = Scene_TitleScreen;
				scene.StartScene(Scene_TitleScreen);
			}
			break;

		case Scene_Credits:
			// Exit to title screen
			if (Event->key.keysym.sym == SDLK_ESCAPE) {
				Mix_HaltMusic();
				CreditsTimer.stop();
				CurrentScene = Scene_TitleScreen;
				scene.StartScene(Scene_TitleScreen);
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