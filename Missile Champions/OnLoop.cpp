#include <cmath>
#include <iostream>
#include "MChamps.h"

void MChamps::OnLoop() {
	// If starting game from title screen
	if (scene.titleScreen.events.StartGame) {
		scene.titleScreen.StartGameEvent();
		
		if (!scene.titleScreen.events.StartGame) {
			CurrentScene = Scene_CarSelection;
		}
	}

	// Car selection events
	if (scene.carSelection.events.Select) scene.carSelection.SelectEvent();
	if (scene.carSelection.events.SelectP1) scene.carSelection.SelectP1Event();
	if (scene.carSelection.events.SelectP2) {
		scene.carSelection.SelectP2Event();
		
		if (!scene.carSelection.events.SelectP2) {
			CurrentScene = Scene_Gameplay;

			GameBall.resetBall();

			Players[0].SetStartRound();
			Players[1].SetStartRound();

			GameplayCamera.CenterOnCar(Players[0].activeCar);

			RoundStartTimer.start();
			Mix_HaltMusic();
		}
	}

	//// Scene Loop Updates
	switch (CurrentScene) {
	case Scene_Credits:
		if (!CreditsTimer.isStarted()) {
			CreditsTimer.start();
			CreditsY = 0.0;
			CreditsRect->y = 0;
			Mix_PlayMusic(mAssets->music.Credits, 0);
		}

		creditsTicks = CreditsTimer.getTicks();

		if (creditsTicks > 3000 && CreditsY > -672.0) {
			CreditsY -= 0.0075 * timeStep;
			CreditsRect->y = (int)CreditsY;
		}

		if (creditsTicks > 102000) {
			CreditsTimer.stop();
			CurrentScene = Scene_TitleScreen;
		}
		break;

	case Scene_GameOver:
		if (!GameOverTimer.isStarted()) {
			GameOverTimer.start();
		}
		if (GameOverTimer.getTicks() > 5000) {
			GameOverTimer.stop();
			CurrentScene = Scene_TitleScreen;
		}
		break;

	case Scene_TitleScreen:
		// Loop music (because SDL_mixer doesn't seamlessly loop)
		if ((MusicTimer.getTicks() > 6410 || Mix_PlayingMusic() == 0) && !scene.titleScreen.events.StartGame) {
			Mix_PlayMusic(mAssets->music.Title, -1);
			MusicTimer.stop();
			MusicTimer.start();
		}
	
		break;
	
	case Scene_CarSelection:
		// Loop music
		if (MusicTimer.getTicks() > 6400 || Mix_PlayingMusic() == 0) {
			Mix_PlayMusic(mAssets->music.CarSelection, -1);
			MusicTimer.stop();
			MusicTimer.start();
		}
		
		// If there is a move event, move cursor
		if (scene.carSelection.SelectCursor.SelectEvent)
			scene.carSelection.SelectCursor.MoveCursor();

		break;

	case Scene_Gameplay:
		// Fetch current round time
		int startTimerTicks = RoundStartTimer.getTicks();
		
		// If end of goal timer
		if (GoalTimer.getTicks() > 1500) {
			// Stop goal timer
			GoalTimer.stop();
			
			// Set ball to center
			GameBall.resetBall();
			
			// Set cars for kickoff
			Players[0].SetKickoff();
			Players[1].SetKickoff();

			// Center camera on active car
			GameplayCamera.CenterOnCar(Players[0].activeCar);
			
			// If either team scores max points, return to title screen.
			if (Players[0].score == 9 || Players[1].score == 9) {
				// Stop music before switching scenes
				Mix_HaltMusic();

				// Switch to Credits or Game Over scene
				CurrentScene = (Players[0].score == 9 ? Scene_Credits : Scene_GameOver);
				break;
			}
			// Else, start next kickoff.
			else
				RoundStartTimer.start();
		}

		if ((RoundTimer.isStarted() && !RoundTimer.isPaused()) || (startTimerTicks == 0 && RoundStartTimer.isStarted())) {
			if ((MusicTimer.getTicks() >= 57160 || Mix_PlayingMusic() == 0) && (RoundTimer.isStarted() && !RoundTimer.isPaused())) {
				Mix_PlayMusic(mAssets->music.Eurobeat, -1);
				MusicTimer.stop();
				MusicTimer.start();
			}

			// Check Active Car Change
			if (Event_ChangeCar) {
				ActiveCar++;
				if (ActiveCar > 2) ActiveCar = 0;

				// Initialize new active car with current active car controls.
				Players[0].cars[ActiveCar].isBoosting = Players[0].activeCar->isBoosting;
				Players[0].cars[ActiveCar].isJumping = Players[0].activeCar->isJumping;
				Players[0].cars[ActiveCar].MoveDirection = Players[0].activeCar->MoveDirection;
				Players[0].cars[ActiveCar].Turning = Players[0].activeCar->Turning;

				// Swap to new active car.
				Players[0].activeCar = &Players[0].cars[ActiveCar];

				Event_ChangeCar = false;
			}

			//// Car Update
			PlayerCarsUpdate(&Players[0]);
			PlayerCarsUpdate(&Players[1]);

			//// Ball Update
			BallUpdate();

			//// Camera update
			GameplayCamera.CenterOnCar(Players[0].activeCar);

			///// Viewport positioning
			// Update car and child object positions in viewport
			for (int i = 0; i < 2; i++) {
				for (int j = 0; j < 3; j++) {
					// Update car positions in viewport
					Players[i].cars[j].UpdateViewport(&GameplayCamera);
					
					// Update boost streak positions in viewport
					for (int k = 0; k < 5; k++) 
						Players[i].cars[j].streak[k].UpdateViewport(&GameplayCamera);
				}
			}
			
			// Set ball
			GameBall.UpdateViewport(&GameplayCamera);

			// Set ball indicator
			if (GameBall.viewportRect->x < (0 - (GameBall.viewportRect->w / 2))) {
				BallOffscreen = true;
				if (GameBall.viewportRect->y < (0 - (GameBall.viewportRect->h / 2))) {
					BallIndicatorDirection = UP_LEFT;
					BallIndicatorRect->x = 8;
					BallIndicatorRect->y = 8;
				}
				else if (GameBall.viewportRect->y > (176 - 32 - 8 - (GameBall.viewportRect->h / 2))) {
					BallIndicatorDirection = DOWN_LEFT;
					BallIndicatorRect->x = 8;
					BallIndicatorRect->y = 176 - 8 - 32;
				}
				else {
					BallIndicatorDirection = LEFT;
					BallIndicatorRect->x = 8;
					BallIndicatorRect->y = GameBall.viewportRect->y + 16;
				}
			}
			else if (GameBall.viewportRect->x > (256 - 8 - (GameBall.viewportRect->w / 2))) {
				BallOffscreen = true;
				if (GameBall.viewportRect->y < (0 - (GameBall.viewportRect->h / 2))) {
					BallIndicatorDirection = UP_RIGHT;
					BallIndicatorRect->x = 256 - 32 - 8;
					BallIndicatorRect->y = 8;
				}
				else if (GameBall.viewportRect->y > (176 - 32 - 8 - (GameBall.viewportRect->h / 2))) {
					BallIndicatorDirection = DOWN_RIGHT;
					BallIndicatorRect->x = 256 - 32 - 8;
					BallIndicatorRect->y = 176 - 8 - 32;
				}
				else {
					BallIndicatorDirection = RIGHT;
					BallIndicatorRect->x = 256 - 32 - 8;
					BallIndicatorRect->y = GameBall.viewportRect->y + 16;
				}
			}
			else if (GameBall.viewportRect->y < (0 - (GameBall.viewportRect->h / 2)) && GameBall.viewportRect->x >= (0 - (GameBall.viewportRect->w / 2))) {
				BallOffscreen = true;
				BallIndicatorDirection = UP;
				BallIndicatorRect->x = GameBall.viewportRect->x + 16;
				BallIndicatorRect->y = 8;
			}
			else if (GameBall.viewportRect->y > (176 - 32 - 8 - (GameBall.viewportRect->h / 2)) && GameBall.viewportRect->x <= (256 - 8 - (GameBall.viewportRect->w / 2))) {
				BallOffscreen = true;
				BallIndicatorDirection = DOWN;
				BallIndicatorRect->x = GameBall.viewportRect->x + 16;
				BallIndicatorRect->y = 176 - 8 - 32;
			}
			else
				BallOffscreen = false;

			// Set viewport coordinates based on active car
			if (Players[0].activeCar->x <= 112.0)
				Players[0].activeCar->viewportRect->x = (int)Players[0].activeCar->x;
			else if (Players[0].activeCar->x >= 880.0)
				Players[0].activeCar->viewportRect->x = (int)Players[0].activeCar->x - 768;
			if (Players[0].activeCar->y <= 72.0)
				Players[0].activeCar->viewportRect->y = (int)(Players[0].activeCar->y - Players[0].activeCar->z);
			else if (Players[0].activeCar->y >= 280.0)
				Players[0].activeCar->viewportRect->y = (int)(Players[0].activeCar->y - Players[0].activeCar->z) - 208;

			if ((Players[0].activeCar->MoveDirection == Car::Forward ||
				Players[0].activeCar->MoveDirection == Car::Backward) &&
				!Players[0].activeCar->isBoosting) {
				Mix_PlayChannel(CHANNEL_ENGINE, mAssets->sounds.Engine, 0);
			}
			if (Players[0].activeCar->isBoosting && Players[0].activeCar->boostFuel > 0)
				Mix_PlayChannel(CHANNEL_BOOST, mAssets->sounds.Boost, 0);

			// Scored goal
			if (GameBall.cx() < 16) {
				Players[1].score += 1;
				Mix_PlayChannel(CHANNEL_BUZZER, mAssets->sounds.Buzzer, 0);
				RoundTimer.pause();
				GoalTimer.start();
				Mix_HaltMusic();
			}
			if (GameBall.cx() > 1008) {
				Players[0].score += 1;
				Mix_PlayChannel(CHANNEL_TITLESTART, mAssets->sounds.StartSelection, 0);
				RoundTimer.pause();
				GoalTimer.start();
				Mix_HaltMusic();
			}
		}
		
		//// Round Start Countdown
		if (RoundStartTimer.isStarted()) {
			if (startTimerTicks >= 300 && Countdown321 == NULL && CountdownG == NULL) {
				Countdown321 = &mAssets->images.Numbers[3];
				Mix_PlayChannel(CHANNEL_CURSOR, mAssets->sounds.MoveCursor, 0);
			}
			if (startTimerTicks >= 1000 && Countdown321 == &mAssets->images.Numbers[3]) {
				Countdown321 = &mAssets->images.Numbers[2];
				Mix_PlayChannel(CHANNEL_CURSOR, mAssets->sounds.MoveCursor, 0);
			}
			if (startTimerTicks >= 1700 && Countdown321 == &mAssets->images.Numbers[2]) {
				Countdown321 = &mAssets->images.Numbers[1];
				Mix_PlayChannel(CHANNEL_CURSOR, mAssets->sounds.MoveCursor, 0);
			}
			if (startTimerTicks >= 2400 && Countdown321 == &mAssets->images.Numbers[1]) {
				Countdown321 = NULL;
				CountdownG = &mAssets->images.Numbers[6];
				CountdownO = &mAssets->images.Numbers[0];
				Mix_PlayChannel(CHANNEL_TITLESTART, mAssets->sounds.StartSelection, 0);
			}
			if (startTimerTicks >= 3000) {
				CountdownG = NULL;
				CountdownO = NULL;
				if (RoundTimer.isPaused()) {
					RoundTimer.unpause();
				}
				else {
					RoundTimer.start();
				}
				RoundStartTimer.stop();
			}
		}
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
				pow(GameBall.cz() - Players[i].cars[j].cz(), 2)) <= 40.0 &&
				Players[i].cars[j].ballCollide == false) {
				// Play ballhit sound
				Mix_PlayChannel(CHANNEL_BALLHIT, mAssets->sounds.Ballhit, 0);

				// Set colliding flag
				Players[i].cars[j].ballCollide = true;
				
				// Get collision angle in rads (atan2), convert to deg (* 180 / M_PI)
				double newAngle = atan2(GameBall.cy() - Players[i].cars[j].cy(), GameBall.cx() - Players[i].cars[j].cx()) * 180.0 / M_PI;
				double newZX = atan2(GameBall.cx() - Players[i].cars[j].cx(), GameBall.cz() - Players[i].cars[j].cz()) * 180.0 / M_PI;
				
				// Rotate to match axes
				newAngle -= 90.0;
				newZX += 90.0;
				
				// Keep angle between 0 - 359 deg inclusive
				if (newAngle >= 360.0) newAngle -= 360.0;
				if (newAngle < 0.0) newAngle += 360.0;
				// Invert angle
				newAngle = 360.0 - newAngle;
				
				// Keep angle between 0 - 359 deg inclusive
				if (newZX >= 360.0) newZX -= 360.0;
				if (newZX < 0.0) newZX += 360.0;
				// Invert angle
				newZX = 360.0 - newZX;
				
				// Debug output of ball collision angles
				//std::cout << "Ball collision angle: " << newAngle << "\nsin(newAngle): " << sin(newAngle) << "\ncos(newAngle): " << cos(newAngle) << std::endl;
				//std::cout << "Ball cx: " << GameBall.cx() << "\nBall cy: " << GameBall.cy() << "\nBall cz: " << GameBall.cz() << std::endl;
				//std::cout << "Car cx: " << Players[i].cars[j].cx() << "\nCar cy: " << Players[i].cars[j].cy() << "\nCar cz: " << Players[i].cars[j].cz() << std::endl;
				
				// Ball direction set to collision angle
				GameBall.dx = sin(newAngle * M_PI / 180.0);
				GameBall.dy = cos(newAngle * M_PI / 180.0);
				
				// If car colliding is faster on x/y/z, get new dz value
				if (abs(Players[i].cars[j].speed * 1.25) > GameBall.speed || abs(Players[i].cars[j].speed * 1.25) > abs(GameBall.dx))
					GameBall.dz = abs(sin(newZX * M_PI / 180.0)) + abs(cos(newZX * M_PI / 180.0));
				//std::cout << "Ball dz: " << GameBall.dz << std::endl;
				
				// Car speed added to ball speed
				// If car colliding is faster, add to ball speed.
				if(abs(Players[i].cars[j].speed * 1.25) > GameBall.speed)
					GameBall.speed += abs(Players[i].cars[j].speed * 1.25);
			}
			// If car/ball spheres no longer colliding, set collision flag false
			else if (sqrt(
				pow(GameBall.cx() - Players[i].cars[j].cx(), 2) +
				pow(GameBall.cy() - Players[i].cars[j].cy(), 2) +
				pow(GameBall.cz() - Players[i].cars[j].cz(), 2)) > 40.0) {
				Players[i].cars[j].ballCollide = false;
			}
		}
	}

	// Update current ball position
	GameBall.updatePosition(timeStep);

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
		if (ballAnimTicks > ballAnimSpeed) {
			GameBall.ballAnimate.stop();
			GameBall.ballAnimate.start();
		}
	}
}

void MChamps::PlayerCarsUpdate(Player * player) {
	std::uniform_int_distribution<int> badAction(1, 1000);
	for (int i = 0; i < 3; i++) {
		//// AI state set
		// If not active human car
		if (&player->cars[i] != *&Players[0].activeCar) {
			if(badAction(randomEngine) > 999)
				player->cars[i].isBoosting = false;
			if (badAction(randomEngine) > 100) {
				// Get ball XY angle relational to car
				double ballAngle = -atan2(GameBall.cy() - player->cars[i].cy(), GameBall.cx() - player->cars[i].cx()) * 180.0 / M_PI;
				// Rotate to match axes
				ballAngle -= 90.0;
				// Adjust ball angle to within 180 degrees of car forward angle
				if (ballAngle < player->cars[i].angle - 180.0)
					ballAngle += 360.0;
				if (ballAngle >= player->cars[i].angle + 180.0)
					ballAngle -= 360.0;

				//// Determine when to turn
				// Turn to left side of field for positioning if car is P1, and ball is to the left
				if (*&player == &Players[0] && GameBall.cx() - 12 < player->cars[i].cx()) {
					// If left side is closer counterclockwise, turn left.
					if (270.0 > player->cars[i].angle && player->cars[i].angle >= 90.0)
						player->cars[i].Turning = Car::Left;
					// If left side is closer clockwise, turn right.
					else if ((270.0 < player->cars[i].angle && player->cars[i].angle < 360.0) ||
						(0.0 <= player->cars[i].angle && player->cars[i].angle < 90.0))
						player->cars[i].Turning = Car::Right;
					// If facing left, don't turn.
					else
						player->cars[i].Turning = Car::NoTurning;
				}
				// Turn to right side of field for positioning if car is P2, and ball is to the right
				else if (*&player == &Players[1] && GameBall.cx() + 12 > player->cars[i].cx()) {
					// If right side is closer clockwise, turn right.
					if (90.0 <= player->cars[i].angle && player->cars[i].angle < 270.0)
						player->cars[i].Turning = Car::Right;
					// If right side is closer counterclockwise, turn left.
					else if ((270.0 <= player->cars[i].angle && player->cars[i].angle < 360.0) ||
						(90.0 > player->cars[i].angle && player->cars[i].angle >= 0.0))
						player->cars[i].Turning = Car::Left;
					// If facing right, don't turn.
					else
						player->cars[i].Turning = Car::NoTurning;
				}
				else {
					// Set car turn based on ball angle
					if (ballAngle > player->cars[i].angle && ballAngle < player->cars[i].angle + 180.0)
						player->cars[i].Turning = Car::Right;
					else if (ballAngle < player->cars[i].angle && ballAngle >= player->cars[i].angle - 180.0)
						player->cars[i].Turning = Car::Left;
					else
						player->cars[i].Turning = Car::NoTurning;
				}
				// Boost if full boost gague, don't boost if no fuel gague.
				if (!player->cars[i].isBoosting && player->cars[i].boostFuel > (int)((double)MAX_BOOST_FUEL * 0.9))
					player->cars[i].isBoosting = true;
				if (player->cars[i].isBoosting && player->cars[i].boostFuel == 0)
					player->cars[i].isBoosting = false;
			}
			// Move forward, always move forward.
			if (badAction(randomEngine) > 100)
				player->cars[i].MoveDirection = Car::Forward;
			else
				player->cars[i].MoveDirection = Car::NoMovement;
		}

		// Set player.cars[i] speed
		player->cars[i].speed = (player->cars[i].isBoosting && player->cars[i].boostFuel > MIN_BOOST_FUEL ? 0.3 : 0.0);
		if (player->cars[i].MoveDirection == Car::Forward)
			player->cars[i].speed += (player->cars[i].isBoosting && player->cars[i].boostFuel > MIN_BOOST_FUEL ? 0.0 : 0.2);
		else if (player->cars[i].MoveDirection == Car::Backward)
			player->cars[i].speed -= 0.2;

		if (&player->cars[i] != *&Players[0].activeCar)
			player->cars[i].speed *= 0.8;

		// Turn Left
		if (player->cars[i].Turning == Car::Left) {
			player->cars[i].angle += 0.25 * (double)timeStep;
			player->cars[i].dx = sin(player->cars[i].angle * M_PI / 180.0);
			player->cars[i].dy = cos(player->cars[i].angle * M_PI / 180.0);
			// Variation in turning speeds for AI cars, one slower and one faster.
			if (&player->cars[i] == &player->cars[1] && &player->cars[i] != *&player->activeCar)
				player->cars[i].angle += 0.05 * (double)timeStep;
			if (&player->cars[i] == &player->cars[2] && &player->cars[i] != *&player->activeCar)
				player->cars[i].angle -= 0.05 * (double)timeStep;

		}
		// Turn Right
		if (player->cars[i].Turning == Car::Right) {
			player->cars[i].angle -= 0.25 * (double)timeStep;
			player->cars[i].dx = sin(player->cars[i].angle * M_PI / 180.0);
			player->cars[i].dy = cos(player->cars[i].angle * M_PI / 180.0);
			// Variation in turning speeds for AI cars, one slower and one faster.
			if (&player->cars[i] == &player->cars[1] && &player->cars[i] != *&player->activeCar)
				player->cars[i].angle -= 0.05 * (double)timeStep;
			if (&player->cars[i] == &player->cars[2] && &player->cars[i] != *&player->activeCar)
				player->cars[i].angle += 0.05 * (double)timeStep;
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
				player->cars[i].anglesprite = (int)j;
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
				if (BoostRechargeTicks < 750)
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
			player->cars[i].streak[player->cars[i].boostStreakCounter].SpawnSprite(mAssets, &player->cars[i]);

			player->cars[i].boostStreakCounter++;
			if (player->cars[i].boostStreakCounter > 4) 
				player->cars[i].boostStreakCounter = 0;
			
			if (BoostStreakTicks > 50)
				player->cars[i].boostStreakTimer.stop();
			player->cars[i].boostStreakTimer.start();
		}
	}
}