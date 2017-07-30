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
		// Get ticks for shadow blink timer
		Uint32 ShadowTimerTicks = ShadowBlinkTimer.getTicks();
		// Start shadow blink timer if stopped
		if (!ShadowBlinkTimer.isStarted()) {
			ShadowBlinkTimer.start();
		}

		//// Draw field camera
		DrawImage(GameplayCamera.drawarea, GameplayCamera.viewport->rect);

		//// Draw cars & shadows, boost trails, and ball & shadow.
		// Populate car draw list
		int drawCarNum = 0;
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				drawCars[drawCarNum++] = &Players[i].cars[j];
			}
		}

		// Order cars by y-coord ascending
		Car* tempCar;
		for (int i = 1, swapped = 1; (i <= 6) && swapped; i++) {
			swapped = 0;	// If no swap occurs on a pass, exit loop.
			for (int j = 0; j < (6 - 1); j++) {
				if (drawCars[j + 1]->y < drawCars[j]->y) {	// If next car y smaller than current
					tempCar = drawCars[j];					// Store current into temp
					drawCars[j] = drawCars[j + 1];			// Next car stored into current
					drawCars[j + 1] = tempCar;				// Temp car stored into next
					swapped = 1;							// Set swapped flag
				}
			}
		}

		// Draw cars and ball in z-order
		for (int i = 0; i < 6; i++) {
			// Draw ball if before cars
			if (i == 0 && GameBall.y < drawCars[i]->y) DrawBall(ShadowTimerTicks);
			// Draw ball if between cars
			else if (GameBall.y >= drawCars[i - 1]->y && GameBall.y < drawCars[i]->y) DrawBall(ShadowTimerTicks);
			
			// Draw car shadow
			if (ShadowTimerTicks < 16.6 && drawCars[i]->z > 0.0) {
				mAssets->images.CarShadow.rect->x = drawCars[i]->viewportRect->x;
				mAssets->images.CarShadow.rect->y = drawCars[i]->viewportRect->y + (int)drawCars[i]->z;
				DrawImage(&mAssets->images.CarShadow);
			}
			// Draw car boost
			for (int k = 0; k < 5; k++) {
				if (drawCars[i]->streak[k].timeAlive > 0) {
					DrawImage(drawCars[i]->streak[k].image, drawCars[i]->streak[k].viewportRect);
				}
			}
			// Draw car sprite
			DrawImage(drawCars[i]->image, drawCars[i]->viewportRect);
			
			// Draw ball if after last car
			if (i == 5 && GameBall.y >= drawCars[i]->y) DrawBall(ShadowTimerTicks);
		}
		
		// Reset shadow blink timer
		if (ShadowTimerTicks >= 33.3) {
			ShadowBlinkTimer.stop();
		}

		// Draw bottom of field over gameplay objects.
		DrawImage(FieldBottom, GameplayCamera.viewport->rect);

		// Draw UI status bar at bottom over gameplay objects.
		DrawImage(StatusBar);

		// Draw player scores
		DrawImage(&mAssets->images.Numbers[Players[0].score], P1Score);
		DrawImage(&mAssets->images.Numbers[Players[1].score], P2Score);

		Uint32 RoundTicks = RoundTimer.getTicks();

		// Draw Round Timer
		DrawImage(&mAssets->images.Numbers[(RoundTicks / 600000) % 6], Minute10sRect);
		DrawImage(&mAssets->images.Numbers[(RoundTicks / 60000) % 10], Minute1sRect);
		DrawImage(&mAssets->images.Numbers[(RoundTicks / 10000) % 6], Second10sRect);
		DrawImage(&mAssets->images.Numbers[(RoundTicks / 1000) % 10], Second1sRect);

		// Draw boost bar
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

void MChamps::DrawBall(int shadowticks) {
	if (shadowticks < 16.6) {
		mAssets->images.BallShadow.rect->x = GameBall.viewportRect->x;
		mAssets->images.BallShadow.rect->y = GameBall.viewportRect->y + (int)GameBall.z + 8;
		DrawImage(&mAssets->images.BallShadow);
	}
	DrawImage(&GameBall.image[GameBall.frame], GameBall.viewportRect);
}