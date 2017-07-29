#include <cmath>
#include <iostream>
#include "MChamps.h"

void MChamps::OnLoop() {
	// Update timeStep by # of ticks from last cycle
	lastTick = currTick;
	currTick = SDL_GetTicks();
	timeStep = currTick - lastTick;

	if (Event_StartGame) {
		Effect_StartFlashLength += timeStep;

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
			Players[0].activeCar->SetCarSelect(0);
			Players[1].activeCar = &Players[1].cars[0];
			Players[1].activeCar->SetCarSelect(1);
		}
		return;
	}
		

	if (Event_P1Selected) {
		Effect_P1FlashLength += timeStep;
		
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
		Effect_P2FlashLength += timeStep;

		if (Effect_P2FlashLength % 100 > 50 && Effect_P2FlashLength < 450)
			CarSelectBG = &mAssets->images.CarSelectBGP2Flash;
		else
			CarSelectBG = &mAssets->images.CarSelectBGDefault;

		// End flashing if one second of flashing has elapsed.
		if (Effect_P2FlashLength >= 450) {
			CarSelectBG = &mAssets->images.CarSelectBGDefault;
		}

		GameBall.resetBall();

		if (Effect_P2FlashLength >= 1250) {
			Effect_P2FlashLength = 0;
			Event_P2Selected = false;
			CurrentScene = Scene_Gameplay;
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 3; j++) {
					Players[i].cars[j].SetCarKickoff(i, j);
				}
			}
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
				Players[0].activeCar->image = &mAssets->images.CarSprites[Players[0].activeCar->anglesprite][Players[0].team - 1];
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
				Players[1].cars[0].image = &mAssets->images.CarSprites[Players[1].activeCar->anglesprite][Players[1].team - 1];
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
				Players[i].cars[j].viewportRect->y = Players[i].cars[j].y - Players[i].cars[j].z - GameplayCamera.drawarea->rect->y;
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
		GameBall.UpdateViewport(GameplayCamera.drawarea->rect);

		// Set viewport coordinates based on active car
		if (Players[0].activeCar->x <= 112.0)
			Players[0].activeCar->viewportRect->x = (int)Players[0].activeCar->x;
		else if (Players[0].activeCar->x >= 880.0)
			Players[0].activeCar->viewportRect->x = (int)Players[0].activeCar->x - 768;
		if (Players[0].activeCar->y <= 72.0)
			Players[0].activeCar->viewportRect->y = (int)(Players[0].activeCar->y - Players[0].activeCar->z);
		else if (Players[0].activeCar->y >= 280.0)
			Players[0].activeCar->viewportRect->y = (int)(Players[0].activeCar->y - Players[0].activeCar->z) - 208;

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
				pow(GameBall.cy() - Players[i].cars[j].cy(), 2) +
				pow(GameBall.cz() - Players[i].cars[j].cz(), 2)) <= 36.0 &&
				Players[i].cars[j].ballCollide == false) {
				// Set colliding flag
				Players[i].cars[j].ballCollide = true;
				
				// Get collision angle in rads (atan2), convert to deg (* 180 / M_PI)
				double newAngle = atan2(GameBall.cy() - Players[i].cars[j].cy(), GameBall.cx() - Players[i].cars[j].cx()) * 180.0 / M_PI;
				// Rotate to match axes
				newAngle -= 90.0;
				// Keep angle between 0 - 359 deg inclusive
				if (newAngle >= 360.0) newAngle -= 360.0;
				if (newAngle < 0.0) newAngle += 360.0;
				// Invert angle
				newAngle = 360.0 - newAngle;
				// Debug output of ball collision angles
				std::cout << "Ball collision angle: " << newAngle << "\nsin(newAngle): " << sin(newAngle) << "\ncos(newAngle): " << cos(newAngle) << std::endl;
				std::cout << "Ball cx: " << GameBall.cx() << "\nBall cy: " << GameBall.cy() << "\nBall cz: " << GameBall.cz() << std::endl;
				std::cout << "Car cx: " << Players[i].cars[j].cx() << "\nCar cy: " << Players[i].cars[j].cy() << "\nCar cz: " << Players[i].cars[j].cz() << std::endl;
				// Ball direction set to collision angle
				GameBall.dx = sin(newAngle * M_PI / 180.0);
				GameBall.dy = cos(newAngle * M_PI / 180.0);
				// Car speed added to ball speed
				GameBall.speed += abs(Players[i].cars[j].speed * 1.5);
			}
			// If car/ball spheres no longer colliding, set collision flag false
			else if (sqrt(
				pow(GameBall.cx() - Players[i].cars[j].cx(), 2) +
				pow(GameBall.cy() - Players[i].cars[j].cy(), 2)) > 40.0) {
				Players[i].cars[j].ballCollide = false;
			}
		}
	}
	// Update current ball speed
	GameBall.updateSpeed(timeStep);

	// Move ball
	GameBall.x += GameBall.dx * GameBall.speed * (double)timeStep;
	GameBall.y += GameBall.dy * GameBall.speed * (double)timeStep;

	double moveBallX = 0.0;
	double moveBallY = 0.0;

	// Inner wall collision
	if (GameBall.x < 32.0 && (GameBall.cy() < 116.0 || GameBall.cy() > 236.0 + 48.0)) {
		moveBallX = 32.0;
	}
	if (GameBall.x > 1024.0 - 32.0 - 48.0 && (GameBall.cy() < 116.0 || GameBall.cy() > 236.0 + 48.0)) {
		moveBallX = 1024.0 - 32.0 - 48.0;
	}
	if (GameBall.y < 116.0 && (GameBall.cx() < 32.0 || GameBall.cx() > 1024.0 - 32.0)) {
		moveBallY = 116.0;
	}
	if (GameBall.y > 236.0 && (GameBall.cx() < 32.0 || GameBall.cx() > 1024.0 - 32.0)) {
		moveBallY = 236.0;
	}
	if (moveBallX > 0.0) { GameBall.dx *= -1.0; GameBall.x = moveBallX; }
	if (moveBallY > 0.0) { GameBall.dy *= -1.0; GameBall.y = moveBallY; }

	// Outer boundary collision
	if (GameBall.x < -24.0) {
		GameBall.x = -24.0;
		GameBall.dx *= -1.0;
	}
	if (GameBall.x > 1000.0) {
		GameBall.x = 1000.0;
		GameBall.dx *= -1.0;
	}
	if (GameBall.y < 20.0) {
		GameBall.y = 20.0;
		GameBall.dy *= -1.0;
	}
	if (GameBall.y > 348.0) {
		GameBall.y = 348.0;
		GameBall.dy *= -1.0;
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
			player->cars[i].angle += 0.25 * (double)timeStep;
			player->cars[i].dx = sin(player->cars[i].angle * M_PI / 180.0);
			player->cars[i].dy = cos(player->cars[i].angle * M_PI / 180.0);
		}
		// Turn Right
		if (player->cars[i].Turning == Car::Right) {
			player->cars[i].angle -= 0.25 * (double)timeStep;
			player->cars[i].dx = sin(player->cars[i].angle * M_PI / 180.0);
			player->cars[i].dy = cos(player->cars[i].angle * M_PI / 180.0);
		}

		// Keep rotation angle within 0 - 360		
		if (player->cars[i].angle >= 360.0) player->cars[i].angle -= 360.0;
		if (player->cars[i].angle < 0.0) player->cars[i].angle += 360.0;

		// Jumping stuff
		if (player->cars[i].isJumping) {
			player->cars[i].jumpTimer.start();
			player->cars[i].isJumping = false;
		}
		Uint32 jumpTicks = player->cars[i].jumpTimer.getTicks();
		if (player->cars[i].jumpTimer.isStarted()) {
			player->cars[i].dz = .15;
			if (jumpTicks >= 300) {
				player->cars[i].jumpTimer.stop();
			}
		}
		if (player->cars[i].z > 0.0 && !player->cars[i].jumpTimer.isStarted()) {
			player->cars[i].dz = -0.15;
		}

		// Move player.cars[i]
		player->cars[i].x += player->cars[i].dx * player->cars[i].speed * (double)timeStep;
		player->cars[i].y += player->cars[i].dy * player->cars[i].speed * (double)timeStep;
		player->cars[i].z += player->cars[i].dz * (double)timeStep;

		if (player->cars[i].z < 0.0) {
			player->cars[i].z = 0.0;
			player->cars[i].dz = 0.0;
		}

		double moveCarX = 0.0;
		double moveCarY = 0.0;

		// Inner wall collision
		if (player->cars[i].x < 32.0 && (player->cars[i].cy() < 116.0 || player->cars[i].cy() > 244.0 + 32.0)) {
			moveCarX = 32.0;
		}
		if (player->cars[i].x > 1024.0 - 64.0 && (player->cars[i].cy() < 116.0 || player->cars[i].cy() > 244.0 + 32.0)) {
			moveCarX = 1024.0 - 64.0;
		}
		if (player->cars[i].y < 116.0 && (player->cars[i].cx() < 32.0 || player->cars[i].cx() > 1024.0 - 32.0)) {
			moveCarY = 116.0;
		}
		if (player->cars[i].y > 244.0 && (player->cars[i].cx() < 32.0 || player->cars[i].cx() > 1024.0 - 32.0)) {
			moveCarY = 244.0;
		}
		if (moveCarX > 0.0) { player->cars[i].x = moveCarX; }
		if (moveCarY > 0.0) { player->cars[i].y = moveCarY; }

		// Outer boundary collision
		if (player->cars[i].x < 0.0)
			player->cars[i].x = 0.0;
		if (player->cars[i].x > 992.0)
			player->cars[i].x = 992.0;
		if (player->cars[i].y < 20.0)
			player->cars[i].y = 20.0;
		if (player->cars[i].y > 356.0)
			player->cars[i].y = 356.0;

		// Set display angle of player.cars[i] sprite
		for (double a = 11.25, j = 0.0; a <= 371.25; a += 22.5, j++) {
			if (player->cars[i].angle < a && player->cars[i].angle >= (a - 22.5)) {
				player->cars[i].anglesprite = j;
			}
			if (j == 15.0) j = -1.0;
		}

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
				player->cars[i].x, player->cars[i].y - Players[0].cars[i].z, player->cars[i].angle, mAssets);

			player->cars[i].boostStreakCounter++;
			if (player->cars[i].boostStreakCounter > 4) 
				player->cars[i].boostStreakCounter = 0;
			
			if (BoostStreakTicks > 50)
				player->cars[i].boostStreakTimer.stop();
			player->cars[i].boostStreakTimer.start();
		}
	}
}