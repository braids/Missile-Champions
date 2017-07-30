#include "MChamps.h"

void MChamps::OnRender() {
	// Clear buffer
	mGraphics->ClearBackBuffer();
	
	// Draw scene
	switch (CurrentScene) {
	case Scene_TitleScreen:
		// Draw title screen background
		DrawImage(TitleScreenBG);
		break;

	case Scene_CarSelection:
		// Draw current car selection background
		DrawImage(CarSelectBG);
		// Draw selection cursor if team selections are not finished
		if (Players[1].team == 0)
			DrawImage(CarSelectionCursor.image);
		// Draw player 1 car and border when selected
		if (Players[0].team > 0) {
			DrawImage(Players[0].activeCar->image, Players[0].activeCar->viewportRect);
			DrawImage(&mAssets->images.CarSelectWindowSprites[Players[0].team - 1], Players[0].selectionViewport);
		}
		// Draw player 2 car and border when selected
		if (Players[1].team > 0) {
			DrawImage(Players[1].activeCar->image, Players[1].activeCar->viewportRect);
			DrawImage(&mAssets->images.CarSelectWindowSprites[Players[1].team - 1], Players[1].selectionViewport);
		}
		break;

	case Scene_Gameplay:
		// Draw field camera
		DrawImage(GameplayCamera.drawarea, GameplayCamera.viewport->rect);
		/*
		// Draw boost lines
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				for (int k = 0; k < 5; k++) {
					if (Players[i].cars[j].streak[k].timeAlive > 0) {
						DrawImage(Players[i].cars[j].streak[k].image, Players[i].cars[j].streak[k].viewportRect);
					}
				}
			}
		}
		*/

		Uint32 ShadowTimerTicks = ShadowBlinkTimer.getTicks();
		if (!ShadowBlinkTimer.isStarted()) {
			ShadowBlinkTimer.start();
		}
		// Draw cars
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				if (ShadowTimerTicks < 16.6 && Players[i].cars[j].z > 0.0) {
					mAssets->images.CarShadow.rect->x = Players[i].cars[j].viewportRect->x;
					mAssets->images.CarShadow.rect->y = Players[i].cars[j].viewportRect->y + (int)Players[i].cars[j].z;
					DrawImage(&mAssets->images.CarShadow);
				}
				for (int k = 0; k < 5; k++) {
					if (Players[i].cars[j].streak[k].timeAlive > 0) {
						DrawImage(Players[i].cars[j].streak[k].image, Players[i].cars[j].streak[k].viewportRect);
					}
				}
				DrawImage(Players[i].cars[j].image, Players[i].cars[j].viewportRect);
			}
		}

		if (ShadowTimerTicks < 16.6) {
			mAssets->images.BallShadow.rect->x = GameBall.viewportRect->x;
			mAssets->images.BallShadow.rect->y = GameBall.viewportRect->y + (int)GameBall.z + 8;
			DrawImage(&mAssets->images.BallShadow);
		}
		DrawImage(&GameBall.image[GameBall.frame], GameBall.viewportRect);
		

		if (ShadowTimerTicks >= 33.3) {
			ShadowBlinkTimer.stop();
		}

		/* Disabling debug lines
		// Draw angle line on active car
		mGraphics->DrawLine(255, 0, 0, 
			(int)Players[0].activeCar->viewportRect->x + 16,
			(int)Players[0].activeCar->viewportRect->y + 16,
			(int)Players[0].activeCar->viewportRect->x + 16 + (int)(Players[0].activeCar->dx * 20.0),
			(int)Players[0].activeCar->viewportRect->y + 16 + (int)(Players[0].activeCar->dy * 20.0));
		
		// Draw angle line on active car
		mGraphics->DrawLine(255, 0, 0,
			(int)GameBall.viewportRect->x + 24,
			(int)GameBall.viewportRect->y + 24,
			(int)GameBall.viewportRect->x + 24 + (int)(GameBall.dx * GameBall.speed * 48.0),
			(int)GameBall.viewportRect->y + 24 + (int)(GameBall.dy * GameBall.speed * 48.0));
			*/

		// Draw bottom of field over gameplay objects.
		DrawImage(FieldBottom, GameplayCamera.viewport->rect);

		// Draw UI status bar at bottom over gameplay objects.
		DrawImage(StatusBar);

		BoostBarScaleRect->w = (int)(64.0 * ((double)Players[0].activeCar->boostFuel / (double)MAX_BOOST_FUEL));
		DrawImage(BoostBar, BoostBarScaleRect);

		break;
	}

	// Render dat
	mGraphics->Render();
}

// Draw an Asset::Image to the renderer
void MChamps::DrawImage(Assets::Image* image) {
	mGraphics->DrawTexture(image->texture, image->rect);
}

// Draw an Asset::Image to the renderer at a specific SDL_Rect
void MChamps::DrawImage(Assets::Image* image, SDL_Rect* drawRect) {
	mGraphics->DrawTextureAtLocation(image->texture, image->rect, drawRect);
}