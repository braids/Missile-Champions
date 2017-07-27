#include <cmath>
#include <iostream>
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
				sin(90 * M_PI / 180), // dx
				cos(90 * M_PI / 180), // dy
				90, 0, // Angle and speed
				false,
				false,
				0, MAX_BOOST_FUEL,
				Car::NoMovement,
				Car::NoTurning
			};
			Players[0].cars[1] = {
				&mAssets->images.CarSprites[1][Players[0].team - 1],
				Players[0].cars[1].viewportRect,
				1, // Image and sprite angle
				98, 122, 0, // x/y
				sin(90 * M_PI / 180), // dx
				cos(90 * M_PI / 180), // dy
				90, 0, // Angle and speed
				false,
				false,
				0, MAX_BOOST_FUEL,
				Car::Backward,
				Car::Right
			};
			Players[0].cars[2] = {
				&mAssets->images.CarSprites[1][Players[0].team - 1],
				Players[0].cars[2].viewportRect,
				1, // Image and sprite angle
				48, 112, 0, // x/y
				sin(90 * M_PI / 180), // dx
				cos(90 * M_PI / 180), // dy
				90, 0, // Angle and speed
				false,
				false,
				0, MAX_BOOST_FUEL,
				Car::NoMovement,
				Car::NoTurning
			};
			Players[1].cars[0] = {
				&mAssets->images.CarSprites[3][Players[1].team - 1],
				Players[1].cars[0].viewportRect,
				3, // Image and sprite angle
				208, 72, 0, // x/y
				sin(270 * M_PI / 180), // dx
				cos(270 * M_PI / 180), // dy
				270, 0, // Angle and speed
				false,
				false,
				0, MAX_BOOST_FUEL,
				Car::Forward,
				Car::Left
			};
			Players[1].cars[1] = {
				&mAssets->images.CarSprites[3][Players[1].team - 1],
				Players[1].cars[1].viewportRect,
				3, // Image and sprite angle
				308, 172, 0, // x/y
				sin(270 * M_PI / 180), // dx
				cos(270 * M_PI / 180), // dy
				270, 0, // Angle and speed
				false,
				false,
				0, MAX_BOOST_FUEL,
				Car::NoMovement,
				Car::NoTurning
			};
			Players[1].cars[2] = {
				&mAssets->images.CarSprites[3][Players[1].team - 1],
				Players[1].cars[2].viewportRect,
				3, // Image and sprite angle
				408, 272, 0, // x/y
				sin(270 * M_PI / 180), // dx
				cos(270 * M_PI / 180), // dy
				270, 0, // Angle and speed
				false,
				false,
				0, MAX_BOOST_FUEL,
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
		
		//// Ball Update
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
		// Update car and child object positions in viewport
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				// Update car positions in viewport
				Players[i].cars[j].viewportRect->x = Players[i].cars[j].x - GameplayCamera.drawarea->rect->x;
				Players[i].cars[j].viewportRect->y = Players[i].cars[j].y - GameplayCamera.drawarea->rect->y;
				// Update boost streak positions in viewport
				for (int k = 0; k < 5; k++) {
					if (Players[i].cars[j].streak[k].timeAlive > 0) {
						Players[i].cars[j].streak[k].viewportRect->x = Players[i].cars[j].streak[k].x - GameplayCamera.drawarea->rect->x;
						Players[i].cars[j].streak[k].viewportRect->y = Players[i].cars[j].streak[k].y - GameplayCamera.drawarea->rect->y;
					}
				}
			}
		}
		// Set ball
		GameBall.viewportRect->x = GameBall.x - GameplayCamera.drawarea->rect->x;
		GameBall.viewportRect->y = GameBall.y - GameplayCamera.drawarea->rect->y;
		// Set viewport coordinates based on active car
		if (Players[0].activeCar->x <= 112)
			Players[0].activeCar->viewportRect->x = (int)Players[0].activeCar->x;
		else if (Players[0].activeCar->x >= 880)
			Players[0].activeCar->viewportRect->x = (int)Players[0].activeCar->x - 768;
		if (Players[0].activeCar->y <= 72)
			Players[0].activeCar->viewportRect->y = (int)Players[0].activeCar->y;
		else if (Players[0].activeCar->y >= 280)
			Players[0].activeCar->viewportRect->y = (int)Players[0].activeCar->y - 208;

		break;
	}	
}

void MChamps::BallUpdate() {
	// Ball Collision
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			// Check sphere collision between car and ball if not currently colliding
			if (sqrt(
				pow(GameBall.cx() - Players[i].cars[j].cx(), 2) +
				pow(GameBall.cy() - Players[i].cars[j].cy(), 2)) <= 40 &&
				Players[i].cars[j].ballCollide == false) {
				// Set colliding flag
				Players[i].cars[j].ballCollide = true;
				
				// Get collision angle in rads (atan2), convert to deg (* 180 / M_PI)
				double newAngle = atan2(GameBall.cy() - Players[i].cars[j].cy(), GameBall.cx() - Players[i].cars[j].cx()) * 180 / M_PI;
				// Rotate to match axes
				newAngle -= 90;
				// Keep angle between 0 - 359 deg inclusive
				if (newAngle >= 360) newAngle -= 360;
				if (newAngle < 0) newAngle += 360;
				// Invert angle
				newAngle = 360 - newAngle;
				// Debug output of ball collision angles
				std::cout << "Ball collision angle: " << newAngle << "\nsin(newAngle): " << sin(newAngle) << "\ncos(newAngle): " << cos(newAngle) << std::endl;
				// Ball direction set to collision angle
				GameBall.dx = sin(newAngle * M_PI / 180);
				GameBall.dy = cos(newAngle * M_PI / 180);
				// Car speed added to ball speed
				GameBall.speed += abs(Players[i].cars[j].speed * 1.5);
			}
			// If car/ball spheres no longer colliding, set collision flag false
			else if (sqrt(
				pow(GameBall.cx() - Players[i].cars[j].cx(), 2) +
				pow(GameBall.cy() - Players[i].cars[j].cy(), 2)) > 40) {
				Players[i].cars[j].ballCollide = false;
			}
		}
	}
	// Update current ball speed
	GameBall.updateSpeed(timeStep);

	// Move ball
	GameBall.x += GameBall.dx * GameBall.speed * timeStep;
	GameBall.y += GameBall.dy * GameBall.speed * timeStep;

	double moveBallX = 0;
	double moveBallY = 0;

	// Inner wall collision
	if (GameBall.x < 32 && (GameBall.cy() < 116 || GameBall.cy() > 236 + 48)) {
		moveBallX = 32;
	}
	if (GameBall.x > 1024 - 32 - 48 && (GameBall.cy() < 116 || GameBall.cy() > 236 + 48)) {
		moveBallX = 1024 - 32 - 48;
	}
	if (GameBall.y < 116 && (GameBall.cx() < 32 || GameBall.cx() > 1024 - 32)) {
		moveBallY = 116;
	}
	if (GameBall.y > 236 && (GameBall.cx() < 32 || GameBall.cx() > 1024 - 32)) {
		moveBallY = 236;
	}
	if (moveBallX > 0) { GameBall.dx *= -1; GameBall.x = moveBallX; }
	if (moveBallY > 0) { GameBall.dy *= -1; GameBall.y = moveBallY; }

	// Outer boundary collision
	if (GameBall.x < -24) {
		GameBall.x = -24;
		GameBall.dx *= -1;
	}
	if (GameBall.x > 1000) {
		GameBall.x = 1000;
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

	// Ball Sprite Update
	Uint32 ballAnimTicks = GameBall.ballAnimate.getTicks();
	Uint32 ballAnimSpeed = 1000 - (Uint32)(1000.0 * GameBall.speed * 1.5); // low: -0, high: -350
	if (GameBall.ballAnimate.isStarted()) {
		if (GameBall.dx < 0.0 && GameBall.speed > 0.0) {
			if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.25)) GameBall.frame = 3;
			else if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.5)) GameBall.frame = 2;
			else if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.75)) GameBall.frame = 1;
			else GameBall.frame = 0;
		}
		if (GameBall.dx >= 0.0 && GameBall.speed > 0.0) {
			if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.25)) GameBall.frame = 1;
			else if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.5)) GameBall.frame = 2;
			else if (ballAnimTicks % ballAnimSpeed < (Uint32)((double)ballAnimSpeed * 0.75)) GameBall.frame = 3;
			else GameBall.frame = 0;
		}
	}
}

void MChamps::PlayerCarsUpdate(Player * player) {
	for (int i = 0; i < 3; i++) {
		// Set player.cars[i] speed
		player->cars[i].speed = (player->cars[i].isBoosting && player->cars[i].boostFuel > MIN_BOOST_FUEL ? 0.3 : 0.0);
		if (player->cars[i].MoveDirection == Car::Forward)
			player->cars[i].speed += (player->cars[i].isBoosting && player->cars[i].boostFuel > MIN_BOOST_FUEL ? 0.0 : 0.2);
		else if (player->cars[i].MoveDirection == Car::Backward)
			player->cars[i].speed -= 0.2;

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

		double moveCarX = 0;
		double moveCarY = 0;

		// Inner wall collision
		if (player->cars[i].x < 32 && (player->cars[i].cy() < 116 || player->cars[i].cy() > 244 + 32)) {
			moveCarX = 32;
		}
		if (player->cars[i].x > 1024 - 64 && (player->cars[i].cy() < 116 || player->cars[i].cy() > 244 + 32)) {
			moveCarX = 1024 - 64;
		}
		if (player->cars[i].y < 116 && (player->cars[i].cx() < 32 || player->cars[i].cx() > 1024 - 32)) {
			moveCarY = 116;
		}
		if (player->cars[i].y > 244 && (player->cars[i].cx() < 32 || player->cars[i].cx() > 1024 - 32)) {
			moveCarY = 244;
		}
		if (moveCarX > 0) { player->cars[i].x = moveCarX; }
		if (moveCarY > 0) { player->cars[i].y = moveCarY; }

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

		//// Boost Streak
		// Update existing
		Uint32 BoostStreakTicks = player->cars[i].boostStreakTimer.getTicks();
		Uint32 BoostRechargeTicks = player->cars[i].boostRechargeTimer.getTicks();
		for (int j = 0; j < 5; j++) {
			if (player->cars[i].streak[j].timeAlive > 0) {
				player->cars[i].streak[j].UpdateDecaySprite(timeStep);
				if(player->cars[i].streak[j].decaySprite == 1)
					player->cars[i].streak[j].image = &mAssets->images.BoostF1Sprite[player->cars[i].streak[j].angleSprite];
				if (player->cars[i].streak[j].decaySprite == 2)
					player->cars[i].streak[j].image = &mAssets->images.BoostF2Sprite[player->cars[i].streak[j].angleSprite];
			}
		}

		if (!player->cars[i].isBoosting) {
			if (player->cars[i].boostStreakTimer.isStarted()) {
				player->cars[i].boostStreakTimer.stop();
				player->cars[i].boostRechargeTimer.start();
			}
			
			if (player->cars[i].boostFuel < MAX_BOOST_FUEL) {
				if (BoostRechargeTicks < 2000)
					player->cars[i].boostFuel += 1 * timeStep;
				else
					player->cars[i].boostFuel += 5 * timeStep;
			}

			if (player->cars[i].boostFuel > MAX_BOOST_FUEL) {
				player->cars[i].boostRechargeTimer.stop();
				player->cars[i].boostFuel = MAX_BOOST_FUEL;
			}
				
		}

		if (player->cars[i].isBoosting && player->cars[i].boostFuel > MIN_BOOST_FUEL) {
			player->cars[i].boostRechargeTimer.stop();
			player->cars[i].boostFuel -= 2 * timeStep;
			if (player->cars[i].boostFuel < MIN_BOOST_FUEL)
				player->cars[i].boostFuel = MIN_BOOST_FUEL;
		}

		if (player->cars[i].isBoosting && player->cars[i].boostFuel > MIN_BOOST_FUEL && (!player->cars[i].boostStreakTimer.isStarted() || BoostStreakTicks > 50)) {
			player->cars[i].streak[player->cars[i].boostStreakCounter].SpawnSprite(
				player->cars[i].x, player->cars[i].y, player->cars[i].angle, mAssets);

			player->cars[i].boostStreakCounter++;
			if (player->cars[i].boostStreakCounter > 4) 
				player->cars[i].boostStreakCounter = 0;
			
			if (BoostStreakTicks > 50)
				player->cars[i].boostStreakTimer.stop();
			player->cars[i].boostStreakTimer.start();
		}
	}
}