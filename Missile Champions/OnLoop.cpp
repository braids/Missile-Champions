#include <cmath>
#include "MChamps.h"

void MChamps::OnLoop() {
	// Update timeStep by # of ticks from last cycle
	lastTick = currTick;
	currTick = SDL_GetTicks();
	timeStep = (float) currTick - (float) lastTick;

	if (Event_StartGame) {
		Effect_StartFlashLength += (Uint32) timeStep;

		if (Effect_StartFlashLength % 400 > 200 || Effect_StartFlashLength >= 1200)
			TitleScreenBG = &mAssets->images.TitleScreenStartVisible;
		else
			TitleScreenBG = &mAssets->images.TitleScreenStartHidden;
		
		if (Effect_StartFlashLength >= 2000) {
			Effect_StartFlashLength = 0;
			Event_StartGame = false;
			
			CurrentScene = Scene_CarSelection;
			
			Players[0].team = 0;
			Players[1].team = 0;
			CarSelectionCursor.column = 0;
			CarSelectionCursor.row = 0;
			CarSelectionCursor.image->rect->x = 16;
			CarSelectionCursor.image->rect->y = 64;
			Players[0].activeCar = &Players[0].cars[0];
			Players[0].activeCar->viewportRect->x = 56;
			Players[0].activeCar->viewportRect->y = 24;
			Players[1].activeCar = &Players[1].cars[0];
			Players[1].activeCar->viewportRect->x = 168;
			Players[1].activeCar->viewportRect->y = 24;
		}
		return;
	}
		

	if (Event_P1Selected) {
		Effect_P1FlashLength += (Uint32) timeStep;
		
		if (Effect_P1FlashLength % 100 > 50)
			CarSelectBG = &mAssets->images.CarSelectBGP1Flash;
		else
			CarSelectBG = &mAssets->images.CarSelectBGDefault;

		// End flashing if one second of flashing has elapsed.
		if (Effect_P1FlashLength >= 450) {
			Effect_P1FlashLength = 0;
			Event_P1Selected = false;
			CarSelectBG = &mAssets->images.CarSelectBGDefault;
		}
	}

	if (Event_P2Selected) {
		Effect_P2FlashLength += (Uint32) timeStep;

		if (Effect_P2FlashLength % 100 > 50 && Effect_P2FlashLength < 450)
			CarSelectBG = &mAssets->images.CarSelectBGP2Flash;
		else
			CarSelectBG = &mAssets->images.CarSelectBGDefault;

		// End flashing if one second of flashing has elapsed.
		if (Effect_P2FlashLength >= 450) {
			CarSelectBG = &mAssets->images.CarSelectBGDefault;
		}

		GameBall.x = 100;
		GameBall.y = 250;
		GameBall.dx = 0;
		GameBall.dy = 0;
		GameBall.speed = 0;
		GameBall.viewportRect->x = 100;
		GameBall.viewportRect->y = 250;

		if (Effect_P2FlashLength >= 1250) {
			Effect_P2FlashLength = 0;
			Event_P2Selected = false;
			CurrentScene = Scene_Gameplay;
			Players[0].cars[0] = { 
				&mAssets->images.CarSprites[1][Players[0].team - 1], 
				Players[0].cars[0].viewportRect,
				1, // Image and sprite angle
				48, 72, 0, // x/y
				48, 72, 0, // viewport x/y
				sin(90 * M_PI / 180), // dx
				cos(90 * M_PI / 180), // dy
				90, 0, // Angle and speed
				false,
				Car::NoMovement, 
				Car::NoTurning
			};
			Players[0].cars[1] = {
				&mAssets->images.CarSprites[1][Players[0].team - 1],
				Players[0].cars[1].viewportRect,
				1, // Image and sprite angle
				98, 122, 0, // x/y
				98, 122, 0, // viewport x/y
				sin(90 * M_PI / 180), // dx
				cos(90 * M_PI / 180), // dy
				90, 0, // Angle and speed
				false,
				Car::Backward,
				Car::Right
			};
			Players[0].cars[2] = {
				&mAssets->images.CarSprites[1][Players[0].team - 1],
				Players[0].cars[2].viewportRect,
				1, // Image and sprite angle
				48, 112, 0, // x/y
				48, 112, 0, // viewport x/y
				sin(90 * M_PI / 180), // dx
				cos(90 * M_PI / 180), // dy
				90, 0, // Angle and speed
				false,
				Car::NoMovement,
				Car::NoTurning
			};
			Players[1].cars[0] = {
				&mAssets->images.CarSprites[3][Players[1].team - 1],
				Players[1].cars[0].viewportRect,
				3, // Image and sprite angle
				208, 72, 0, // x/y
				208, 72, 0, // viewport x/y
				sin(270 * M_PI / 180), // dx
				cos(270 * M_PI / 180), // dy
				270, 0, // Angle and speed
				false,
				Car::Forward,
				Car::Left
			};
			Players[1].cars[1] = {
				&mAssets->images.CarSprites[3][Players[1].team - 1],
				Players[1].cars[1].viewportRect,
				3, // Image and sprite angle
				308, 172, 0, // x/y
				308, 172, 0, // viewport x/y
				sin(270 * M_PI / 180), // dx
				cos(270 * M_PI / 180), // dy
				270, 0, // Angle and speed
				false,
				Car::NoMovement,
				Car::NoTurning
			};
			Players[1].cars[2] = {
				&mAssets->images.CarSprites[3][Players[1].team - 1],
				Players[1].cars[2].viewportRect,
				3, // Image and sprite angle
				408, 272, 0, // x/y
				408, 272, 0, // viewport x/y
				sin(270 * M_PI / 180), // dx
				cos(270 * M_PI / 180), // dy
				270, 0, // Angle and speed
				false,
				Car::NoMovement,
				Car::NoTurning
			};
			GameplayCamera.drawarea->rect->x = ((int)Players[0].activeCar->x + (Players[0].activeCar->image->rect->w / 2)) - (GameplayCamera.drawarea->rect->w / 2);
			GameplayCamera.drawarea->rect->y = ((int)Players[0].activeCar->y + (Players[0].activeCar->image->rect->h / 2)) - (GameplayCamera.drawarea->rect->h / 2);
		}
	}

	//// Scene Loop Updates
	switch (CurrentScene) {
	case Scene_CarSelection:
		if (Event_CarSelected) {
			if (Players[0].team == 0) {
				Event_P1Selected = true;
				if (CarSelectionCursor.column == 0) {
					Players[0].team = CarSelectionCursor.row + 1;
				}
				else {
					Players[0].team = CarSelectionCursor.row + 5;
				}
				Players[0].activeCar->image = &mAssets->images.CarSprites[1][Players[0].team - 1];
				CarSelectionCursor.column = 0;
				CarSelectionCursor.row = 0;
				CarSelectionCursor.image->rect->x = 128;
				CarSelectionCursor.image->rect->y = 64;
			}
			else if (Players[1].team == 0) {
				Event_P2Selected = true;
				if (CarSelectionCursor.column == 0) {
					Players[1].team = CarSelectionCursor.row + 1;
				}
				else {
					Players[1].team = CarSelectionCursor.row + 5;
				}
				Players[1].cars[0].image = &mAssets->images.CarSprites[3][Players[1].team - 1];
			}
			Event_CarSelected = false;
		}
		
		if (CarSelectionCursor.SelectEvent & CarSelectionCursor.SelectUp) {
			CarSelectionCursor.row -= 1;
			if (CarSelectionCursor.row < 0) {
				CarSelectionCursor.row = 3;
				CarSelectionCursor.image->rect->y += 96;
			}
			else
				CarSelectionCursor.image->rect->y -= 32;
		}
		if (CarSelectionCursor.SelectEvent & CarSelectionCursor.SelectDown) {
			CarSelectionCursor.row += 1;
			if (CarSelectionCursor.row > 3) {
				CarSelectionCursor.row = 0;
				CarSelectionCursor.image->rect->y -= 96;
			}
			else
				CarSelectionCursor.image->rect->y += 32;
		}
		if (CarSelectionCursor.SelectEvent & CarSelectionCursor.SelectLeft) {
			CarSelectionCursor.column -= 1;
			if (CarSelectionCursor.column < 0) {
				CarSelectionCursor.column = 1;
				CarSelectionCursor.image->rect->x += 48;
			}
			else
				CarSelectionCursor.image->rect->x -= 48;
		}
		if (CarSelectionCursor.SelectEvent & CarSelectionCursor.SelectRight) {
			CarSelectionCursor.column += 1;
			if (CarSelectionCursor.column > 1) {
				CarSelectionCursor.column = 0;
				CarSelectionCursor.image->rect->x -= 48;
			}
			else
				CarSelectionCursor.image->rect->x += 48;
		}

		// Clear selection inputs
		CarSelectionCursor.SelectEvent = CarSelectionCursor.NoSelection;
		break;

	case Scene_Gameplay:
		// Check Active Car Change
		if (Event_ChangeCar) {
			ActiveCar++;
			if (ActiveCar > 2) ActiveCar = 0;
			Players[0].activeCar = &Players[0].cars[ActiveCar];
			Event_ChangeCar = false;
		}

		//// Car Update
		PlayerCarsUpdate(&Players[0]);
		PlayerCarsUpdate(&Players[1]);
		
		// Ball Update
		BallUpdate();

		//// Camera update
		GameplayCamera.drawarea->rect->x = ((int)Players[0].activeCar->x + 16) - (GameplayCamera.drawarea->rect->w / 2);
		GameplayCamera.drawarea->rect->y = ((int)Players[0].activeCar->y + 16) - (GameplayCamera.drawarea->rect->h / 2);
		// Prevent camera from leaving the level boundary
		if (GameplayCamera.drawarea->rect->x < 0) GameplayCamera.drawarea->rect->x = 0;
		if (GameplayCamera.drawarea->rect->y < 0) GameplayCamera.drawarea->rect->y = 0;
		if (GameplayCamera.drawarea->rect->x > 768) GameplayCamera.drawarea->rect->x = 768;
		if (GameplayCamera.drawarea->rect->y > 208) GameplayCamera.drawarea->rect->y = 208;
		
		///// Viewport positioning
		// Set viewport coordinates based on active car (make it so "target" is followed later)
		if (Players[0].activeCar->x <= 112)
			Players[0].activeCar->vx = Players[0].activeCar->x;
		else if (Players[0].activeCar->x >= 880)
			Players[0].activeCar->vx = Players[0].activeCar->x - 768;
		if (Players[0].activeCar->y <= 72)
			Players[0].activeCar->vy = Players[0].activeCar->y;
		else if (Players[0].activeCar->y >= 280)
			Players[0].activeCar->vy = Players[0].activeCar->y - 208;		
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				Players[i].cars[j].viewportRect->x = Players[i].cars[j].x - GameplayCamera.drawarea->rect->x;
				Players[i].cars[j].viewportRect->y = Players[i].cars[j].y - GameplayCamera.drawarea->rect->y;
			}
		}
		// Set ball
		GameBall.viewportRect->x = GameBall.x - GameplayCamera.drawarea->rect->x;
		GameBall.viewportRect->y = GameBall.y - GameplayCamera.drawarea->rect->y;
		// Set active car draw location in viewport
		Players[0].activeCar->viewportRect->x = (int)Players[0].activeCar->vx;
		Players[0].activeCar->viewportRect->y = (int)Players[0].activeCar->vy;
		

		break;
	}	
}

void MChamps::BallUpdate() {
	// Ball Collision
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			if (sqrt(
				pow(GameBall.cx() - Players[i].cars[j].cx(), 2) +
				pow(GameBall.cy() - Players[i].cars[j].cy(), 2)) < 40 &&
				Players[i].cars[j].ballCollide == false) {
				Players[i].cars[j].ballCollide == true;
				
				double newAngle = atan2(abs(GameBall.cy() - Players[i].cars[j].cy()), abs(GameBall.cx() - Players[i].cars[j].cx()));
				newAngle += 90;
				if (newAngle >= 360) newAngle -= 360;
				if (newAngle < 0) newAngle += 360;
				GameBall.dx = sin(newAngle);
				GameBall.dy = cos(newAngle);
				GameBall.speed += Players[i].cars[j].speed;
			}
			else if (sqrt(
				pow(GameBall.cx() - Players[i].cars[j].cx(), 2) +
				pow(GameBall.cy() - Players[i].cars[j].cy(), 2)) >= 36) {
				Players[i].cars[j].ballCollide == false;
			}
		}
	}
	GameBall.updateSpeed();

	// Move ball
	GameBall.x += GameBall.dx * GameBall.speed * timeStep;
	GameBall.y += GameBall.dy * GameBall.speed * timeStep;

	//if (GameBall.dx < 0 && ) GameBall.frame--;
	//if (GameBall.dx > 0) GameBall.frame++;

	// Outer boundary collision
	if (GameBall.x < 0) {
		GameBall.x = 0;
		GameBall.dx *= -1;
	}
	if (GameBall.x > 976) {
		GameBall.x = 976;
		GameBall.dx *= -1;
	}
	if (GameBall.y < 20) {
		GameBall.y = 20;
		GameBall.dy *= -1;
	}
	if (GameBall.y > 348) {
		GameBall.y = 348;
		GameBall.dy *= -1;
	}


}

void MChamps::PlayerCarsUpdate(Player * player) {
	for (int i = 0; i < 3; i++) {
		// Set player.cars[i] speed
		if (player->cars[i].MoveDirection == Car::Forward)
			player->cars[i].speed = .2;
		else if (player->cars[i].MoveDirection == Car::Backward)
			player->cars[i].speed = -.2;
		else
			player->cars[i].speed = 0;

		// Turn Left
		if (player->cars[i].Turning == Car::Left) {
			player->cars[i].angle += .25 * timeStep;
			player->cars[i].dx = sin(player->cars[i].angle * M_PI / 180);
			player->cars[i].dy = cos(player->cars[i].angle * M_PI / 180);
		}
		// Turn Right
		if (player->cars[i].Turning == Car::Right) {
			player->cars[i].angle -= .25 * timeStep;
			player->cars[i].dx = sin(player->cars[i].angle * M_PI / 180);
			player->cars[i].dy = cos(player->cars[i].angle * M_PI / 180);
		}

		// Keep rotation angle within 0 - 360		
		if (player->cars[i].angle >= 360) player->cars[i].angle -= 360;
		if (player->cars[i].angle < 0) player->cars[i].angle += 360;

		// Move player.cars[i]
		player->cars[i].x += player->cars[i].dx * player->cars[i].speed * timeStep;
		player->cars[i].y += player->cars[i].dy * player->cars[i].speed * timeStep;

		// Inner wall collision
		if (player->cars[i].x < 32 && (player->cars[i].y < 116 || player->cars[i].y > 244)) {
			player->cars[i].x -= player->cars[i].dx * player->cars[i].speed * timeStep;
			player->cars[i].y -= player->cars[i].dy * player->cars[i].speed * timeStep;
		}
		// Outer boundary collision
		if (player->cars[i].x < 0)
			player->cars[i].x = 0;
		if (player->cars[i].x > 992)
			player->cars[i].x = 992;
		if (player->cars[i].y < 20)
			player->cars[i].y = 20;
		if (player->cars[i].y > 356)
			player->cars[i].y = 356;

		// Set display angle of player.cars[i] sprite
		if (player->cars[i].angle >= 315 || player->cars[i].angle < 45)
			player->cars[i].anglesprite = 2;	// Down
		if (player->cars[i].angle >= 45 && player->cars[i].angle < 135)
			player->cars[i].anglesprite = 1;	// Right
		if (player->cars[i].angle >= 135 && player->cars[i].angle < 225)
			player->cars[i].anglesprite = 0;	// Up
		if (player->cars[i].angle >= 225 && player->cars[i].angle < 315)
			player->cars[i].anglesprite = 3;	// Left
		// Update image
		player->cars[i].image = &mAssets->images.CarSprites[player->cars[i].anglesprite][player->team - 1];
	}
}