#include "../../Objects.h"

void SceneManager::Gameplay::Init(Assets* assets, SceneManager* sceneManager) {
	this->parent = sceneManager;
	
	//// Assets
	// Ball
	this->GameBall.initBall(assets->images.BallSprites);

	// Ball indicator
	this->BallIndicatorRect = Graphics::CreateRect(32, 32, 0, 0);

	// Field
	this->FieldBottom = &assets->images.FieldBottom;

	// Camera
	this->GameplayCamera.Init(&assets->images.FieldDrawArea, &assets->images.FieldViewport);
	// Ensure bottom of field tracks with camera
	this->FieldBottom->rect = this->GameplayCamera.drawarea->rect;

	// Numbers
	this->Numbers = assets->images.Numbers;

	// Boost Sprites
	this->BoostSprite = assets->images.BoostSprite;
	this->BoostF1Sprite = assets->images.BoostF1Sprite;
	this->BoostF2Sprite = assets->images.BoostF2Sprite;

	// Countdown Timer
	this->Countdown321 = NULL;
	this->Countdown321Rect = Graphics::CreateRect(8, 8, 124, 100);
	this->CountdownG = NULL;
	this->CountdownGRect = Graphics::CreateRect(8, 8, 120, 100);
	this->CountdownO = NULL;
	this->CountdownORect = Graphics::CreateRect(8, 8, 128, 100);

	// UI
	this->StatusBar = &assets->images.StatusBar;
	this->P1Score = Graphics::CreateRect(8, 8, 40, 192);
	this->P2Score = Graphics::CreateRect(8, 8, 40, 208);
	this->Minute10sRect = Graphics::CreateRect(8, 8, 108, 212);
	this->Minute1sRect = Graphics::CreateRect(8, 8, 116, 212);
	this->Second10sRect = Graphics::CreateRect(8, 8, 132, 212);
	this->Second1sRect = Graphics::CreateRect(8, 8, 140, 212);
	this->BoostBar = &assets->images.BoostBar;
	this->BoostBarScaleRect = Graphics::CreateRect(64, 8, 96, 200);

	// Sounds
	this->SoundMoveCursor = assets->sounds.MoveCursor;
	this->SoundStartSelection = assets->sounds.StartSelection;
	this->SoundEngine = assets->sounds.Engine;
	this->SoundBoost = assets->sounds.Boost;
	this->SoundBuzzer = assets->sounds.Buzzer;
	this->SoundBallhit = assets->sounds.Ballhit;

	// Music
	this->MusicBG = assets->music.Eurobeat;

	//// Vals
	// Events
	this->Event_LeftGoal = false;
	this->Event_RightGoal = false;
	this->Event_ChangeCar = false;

	// Ball indicator
	this->BallOffscreen = false;
	this->BallIndicatorDirection = 0;

	// Round ticks
	this->RoundTicks = 0;
}

void SceneManager::Gameplay::SceneStart() {
	// Ball
	this->GameBall.resetBall();

	// Players
	this->parent->player[0].SetStartRound();
	this->parent->player[1].SetStartRound();

	// Gameplay Camera
	this->GameplayCamera.CenterOnCar(this->parent->player[0].activeCar);

	// Events
	this->Event_LeftGoal = false;
	this->Event_RightGoal = false;
	this->Event_ChangeCar = false;

	// Ball indicator
	this->BallOffscreen = false;
	this->BallIndicatorDirection = 0;

	// Round ticks
	this->RoundTicks = 0;

	// Countdown graphics
	this->Countdown321 = NULL;
	this->CountdownG = NULL;
	this->CountdownO = NULL;

	// Goal timer
	this->GoalTimer.stop();

	// Round start timer
	this->RoundStartTimer.stop();
	this->RoundStartTimer.start();
}

Assets::Image* SceneManager::Gameplay::GetBG() {
	return this->BG;
}

void SceneManager::Gameplay::Update() {
	// Fetch current round time
	int startTimerTicks = this->RoundStartTimer.getTicks();

	// If end of goal timer
	if (this->GoalTimer.getTicks() > 1500) {
		// Stop goal timer
		this->GoalTimer.stop();

		// Set ball to center
		this->GameBall.resetBall();

		// Set cars for kickoff
		this->parent->player[0].SetKickoff();
		this->parent->player[1].SetKickoff();

		// Center camera on active car
		this->GameplayCamera.CenterOnCar(this->parent->player[0].activeCar);

		// If either team scores max points, return to title screen.
		if (this->parent->player[0].score == 9 || this->parent->player[1].score == 9) {
			// Stop music before switching scenes
			Mix_HaltMusic();

			// Switch to Credits or Game Over scene
			this->parent->StartScene(this->parent->player[0].score == 9 ? Scene_Credits : Scene_GameOver);
			return;
		}
		// Else, start next kickoff.
		else
			this->RoundStartTimer.start();
	}

	if ((this->RoundTimer.isStarted() && !this->RoundTimer.isPaused()) || (startTimerTicks == 0 && this->RoundStartTimer.isStarted())) {
		if ((this->MusicTimer.getTicks() >= 57160 || Mix_PlayingMusic() == 0) && (this->RoundTimer.isStarted() && !this->RoundTimer.isPaused())) {
			Mix_PlayMusic(this->MusicBG, -1);
			this->MusicTimer.stop();
			this->MusicTimer.start();
		}

		// Check Active Car Change
		if (this->Event_ChangeCar) {
			this->ActiveCar++;
			if (this->ActiveCar > 2) this->ActiveCar = 0;

			// Initialize new active car with current active car controls.
			this->parent->player[0].cars[this->ActiveCar].isBoosting = this->parent->player[0].activeCar->isBoosting;
			this->parent->player[0].cars[this->ActiveCar].isJumping = this->parent->player[0].activeCar->isJumping;
			this->parent->player[0].cars[this->ActiveCar].MoveDirection = this->parent->player[0].activeCar->MoveDirection;
			this->parent->player[0].cars[this->ActiveCar].Turning = this->parent->player[0].activeCar->Turning;

			// Swap to new active car.
			this->parent->player[0].activeCar = &this->parent->player[0].cars[ActiveCar];

			this->Event_ChangeCar = false;
		}

		//// Car Update
		this->PlayerCarsUpdate(&this->parent->player[0]);
		this->PlayerCarsUpdate(&this->parent->player[1]);

		//// Ball Update
		//this->BallUpdate();
		this->GameBall.CarCollision(this->parent->player);
		this->GameBall.updatePosition(*this->parent->timeStep);
		this->GameBall.UpdateSprite();
		
		//// Camera update
		this->GameplayCamera.CenterOnCar(this->parent->player[0].activeCar);

		///// Viewport positioning
		// Update car and child object positions in viewport
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 3; j++) {
				// Update car positions in viewport
				this->parent->player[i].cars[j].UpdateViewport(&this->GameplayCamera);

				// Update boost streak positions in viewport
				for (int k = 0; k < 5; k++)
					this->parent->player[i].cars[j].streak[k].UpdateViewport(&this->GameplayCamera);
			}
		}

		// Set ball
		this->GameBall.UpdateViewport(&this->GameplayCamera);

		// Set ball indicator
		if (this->GameBall.viewportRect->x < (0 - (this->GameBall.viewportRect->w / 2))) {
			this->BallOffscreen = true;
			if (this->GameBall.viewportRect->y < (0 - (this->GameBall.viewportRect->h / 2))) {
				this->BallIndicatorDirection = UP_LEFT;
				this->BallIndicatorRect->x = 8;
				this->BallIndicatorRect->y = 8;
			}
			else if (this->GameBall.viewportRect->y >(176 - 32 - 8 - (this->GameBall.viewportRect->h / 2))) {
				this->BallIndicatorDirection = DOWN_LEFT;
				this->BallIndicatorRect->x = 8;
				this->BallIndicatorRect->y = 176 - 8 - 32;
			}
			else {
				this->BallIndicatorDirection = LEFT;
				this->BallIndicatorRect->x = 8;
				this->BallIndicatorRect->y = this->GameBall.viewportRect->y + 16;
			}
		}
		else if (this->GameBall.viewportRect->x > (256 - 8 - (this->GameBall.viewportRect->w / 2))) {
			this->BallOffscreen = true;
			if (this->GameBall.viewportRect->y < (0 - (this->GameBall.viewportRect->h / 2))) {
				this->BallIndicatorDirection = UP_RIGHT;
				this->BallIndicatorRect->x = 256 - 32 - 8;
				this->BallIndicatorRect->y = 8;
			}
			else if (this->GameBall.viewportRect->y >(176 - 32 - 8 - (this->GameBall.viewportRect->h / 2))) {
				this->BallIndicatorDirection = DOWN_RIGHT;
				this->BallIndicatorRect->x = 256 - 32 - 8;
				this->BallIndicatorRect->y = 176 - 8 - 32;
			}
			else {
				this->BallIndicatorDirection = RIGHT;
				this->BallIndicatorRect->x = 256 - 32 - 8;
				this->BallIndicatorRect->y = GameBall.viewportRect->y + 16;
			}
		}
		else if (this->GameBall.viewportRect->y < (0 - (this->GameBall.viewportRect->h / 2)) && this->GameBall.viewportRect->x >= (0 - (this->GameBall.viewportRect->w / 2))) {
			this->BallOffscreen = true;
			this->BallIndicatorDirection = UP;
			this->BallIndicatorRect->x = this->GameBall.viewportRect->x + 16;
			this->BallIndicatorRect->y = 8;
		}
		else if (this->GameBall.viewportRect->y >(176 - 32 - 8 - (this->GameBall.viewportRect->h / 2)) && this->GameBall.viewportRect->x <= (256 - 8 - (this->GameBall.viewportRect->w / 2))) {
			this->BallOffscreen = true;
			this->BallIndicatorDirection = DOWN;
			this->BallIndicatorRect->x = this->GameBall.viewportRect->x + 16;
			this->BallIndicatorRect->y = 176 - 8 - 32;
		}
		else
			this->BallOffscreen = false;

		// Set viewport coordinates based on active car
		if (this->parent->player[0].activeCar->x <= 112.0)
			this->parent->player[0].activeCar->viewportRect->x = (int)this->parent->player[0].activeCar->x;
		else if (this->parent->player[0].activeCar->x >= 880.0)
			this->parent->player[0].activeCar->viewportRect->x = (int)this->parent->player[0].activeCar->x - 768;
		if (this->parent->player[0].activeCar->y <= 72.0)
			this->parent->player[0].activeCar->viewportRect->y = (int)(this->parent->player[0].activeCar->y - this->parent->player[0].activeCar->z);
		else if (this->parent->player[0].activeCar->y >= 280.0)
			this->parent->player[0].activeCar->viewportRect->y = (int)(this->parent->player[0].activeCar->y - this->parent->player[0].activeCar->z) - 208;

		if ((this->parent->player[0].activeCar->MoveDirection == Car::Forward ||
			this->parent->player[0].activeCar->MoveDirection == Car::Backward) &&
			!this->parent->player[0].activeCar->isBoosting) {
			Mix_PlayChannel(CHANNEL_ENGINE, this->SoundEngine, 0);
		}
		if (this->parent->player[0].activeCar->isBoosting && this->parent->player[0].activeCar->boostFuel > 0)
			Mix_PlayChannel(CHANNEL_BOOST, this->SoundBoost, 0);

		// Scored goal
		if (this->GameBall.cx() < 16) {
			this->parent->player[1].score += 1;
			Mix_PlayChannel(CHANNEL_BUZZER, this->SoundBuzzer, 0);
			this->RoundTimer.pause();
			this->GoalTimer.start();
			Mix_HaltMusic();
		}
		if (this->GameBall.cx() > 1008) {
			this->parent->player[0].score += 1;
			Mix_PlayChannel(CHANNEL_TITLESTART, this->SoundStartSelection, 0);
			this->RoundTimer.pause();
			this->GoalTimer.start();
			Mix_HaltMusic();
		}
	}

	//// Round Start Countdown
	if (this->RoundStartTimer.isStarted()) {
		if (startTimerTicks >= 300 && this->Countdown321 == NULL && this->CountdownG == NULL) {
			this->Countdown321 = &this->Numbers[3];
			Mix_PlayChannel(CHANNEL_CURSOR, this->SoundMoveCursor, 0);
		}
		if (startTimerTicks >= 1000 && this->Countdown321 == &this->Numbers[3]) {
			this->Countdown321 = &this->Numbers[2];
			Mix_PlayChannel(CHANNEL_CURSOR, this->SoundMoveCursor, 0);
		}
		if (startTimerTicks >= 1700 && this->Countdown321 == &this->Numbers[2]) {
			this->Countdown321 = &this->Numbers[1];
			Mix_PlayChannel(CHANNEL_CURSOR, this->SoundMoveCursor, 0);
		}
		if (startTimerTicks >= 2400 && this->Countdown321 == &this->Numbers[1]) {
			this->Countdown321 = NULL;
			this->CountdownG = &this->Numbers[6];
			this->CountdownO = &this->Numbers[0];
			Mix_PlayChannel(CHANNEL_TITLESTART, this->SoundStartSelection, 0);
		}
		if (startTimerTicks >= 3000) {
			this->CountdownG = NULL;
			this->CountdownO = NULL;
			if (this->RoundTimer.isPaused()) {
				this->RoundTimer.unpause();
			}
			else {
				this->RoundTimer.start();
			}
			this->RoundStartTimer.stop();
		}
	}
}

void SceneManager::Gameplay::PlayerCarsUpdate(Player * player) {
	std::uniform_int_distribution<int> badAction(1, 1000);

	for (int i = 0; i < 3; i++) {
		//// AI state set
		// If not active human car
		if (&player->cars[i] != *&this->parent->player[0].activeCar) {
			if (badAction(this->randomEngine) > 999)
				player->cars[i].isBoosting = false;
			if (badAction(this->randomEngine) > 100) {
				// Get ball XY angle relational to car
				double ballAngle = -atan2(this->GameBall.cy() - player->cars[i].cy(), this->GameBall.cx() - player->cars[i].cx()) * 180.0 / M_PI;
				// Rotate to match axes
				ballAngle -= 90.0;
				// Adjust ball angle to within 180 degrees of car forward angle
				if (ballAngle < player->cars[i].angle - 180.0)
					ballAngle += 360.0;
				if (ballAngle >= player->cars[i].angle + 180.0)
					ballAngle -= 360.0;

				//// Determine when to turn
				// Turn to left side of field for positioning if car is P1, and ball is to the left
				if (*&player == &this->parent->player[0] && GameBall.cx() - 12 < player->cars[i].cx()) {
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
				else if (*&player == &this->parent->player[1] && GameBall.cx() + 12 > player->cars[i].cx()) {
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
				if (!player->cars[i].isBoosting && player->cars[i].boostFuel >(int)((double)MAX_BOOST_FUEL * 0.9))
					player->cars[i].isBoosting = true;
				if (player->cars[i].isBoosting && player->cars[i].boostFuel == 0)
					player->cars[i].isBoosting = false;
			}
			// Move forward, always move forward.
			if (badAction(this->randomEngine) > 100)
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

		if (&player->cars[i] != *&this->parent->player[0].activeCar)
			player->cars[i].speed *= 0.8;

		// Turn Left
		if (player->cars[i].Turning == Car::Left) {
			player->cars[i].angle += 0.25 * (double) *this->parent->timeStep;
			player->cars[i].dx = sin(player->cars[i].angle * M_PI / 180.0);
			player->cars[i].dy = cos(player->cars[i].angle * M_PI / 180.0);
			// Variation in turning speeds for AI cars, one slower and one faster.
			if (&player->cars[i] == &player->cars[1] && &player->cars[i] != *&player->activeCar)
				player->cars[i].angle += 0.05 * (double) *this->parent->timeStep;
			if (&player->cars[i] == &player->cars[2] && &player->cars[i] != *&player->activeCar)
				player->cars[i].angle -= 0.05 * (double) *this->parent->timeStep;

		}
		// Turn Right
		if (player->cars[i].Turning == Car::Right) {
			player->cars[i].angle -= 0.25 * (double) *this->parent->timeStep;
			player->cars[i].dx = sin(player->cars[i].angle * M_PI / 180.0);
			player->cars[i].dy = cos(player->cars[i].angle * M_PI / 180.0);
			// Variation in turning speeds for AI cars, one slower and one faster.
			if (&player->cars[i] == &player->cars[1] && &player->cars[i] != *&player->activeCar)
				player->cars[i].angle -= 0.05 * (double) *this->parent->timeStep;
			if (&player->cars[i] == &player->cars[2] && &player->cars[i] != *&player->activeCar)
				player->cars[i].angle += 0.05 * (double) *this->parent->timeStep;
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
		player->cars[i].x += player->cars[i].dx * player->cars[i].speed * (double) *this->parent->timeStep;
		player->cars[i].y += player->cars[i].dy * player->cars[i].speed * (double) *this->parent->timeStep;
		player->cars[i].z += player->cars[i].dz * (double) *this->parent->timeStep;

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
		player->cars[i].image = &player->cars[i].assets->images.CarSprites[player->cars[i].anglesprite][player->team - 1];

		//// Boost Streak
		// Update existing
		Uint32 BoostStreakTicks = player->cars[i].boostStreakTimer.getTicks();
		Uint32 BoostRechargeTicks = player->cars[i].boostRechargeTimer.getTicks();
		for (int j = 0; j < 5; j++) {
			if (player->cars[i].streak[j].timeAlive > 0) {
				player->cars[i].streak[j].UpdateDecaySprite(*this->parent->timeStep);
				if (player->cars[i].streak[j].decaySprite == 1)
					player->cars[i].streak[j].image = &this->BoostF1Sprite[player->cars[i].streak[j].angleSprite];
				if (player->cars[i].streak[j].decaySprite == 2)
					player->cars[i].streak[j].image = &this->BoostF2Sprite[player->cars[i].streak[j].angleSprite];
			}
		}

		if (!player->cars[i].isBoosting) {
			if (player->cars[i].boostStreakTimer.isStarted()) {
				player->cars[i].boostStreakTimer.stop();
				player->cars[i].boostRechargeTimer.start();
			}

			if (player->cars[i].boostFuel < MAX_BOOST_FUEL) {
				if (BoostRechargeTicks < 750)
					player->cars[i].boostFuel += 1 * *this->parent->timeStep;
				else
					player->cars[i].boostFuel += 5 * *this->parent->timeStep;
			}

			if (player->cars[i].boostFuel > MAX_BOOST_FUEL) {
				player->cars[i].boostRechargeTimer.stop();
				player->cars[i].boostFuel = MAX_BOOST_FUEL;
			}
		}

		if (player->cars[i].isBoosting && player->cars[i].boostFuel > MIN_BOOST_FUEL) {
			player->cars[i].boostRechargeTimer.stop();
			player->cars[i].boostFuel -= 2 * *this->parent->timeStep;
			if (player->cars[i].boostFuel < MIN_BOOST_FUEL)
				player->cars[i].boostFuel = MIN_BOOST_FUEL;
		}

		if (player->cars[i].isBoosting && player->cars[i].boostFuel > MIN_BOOST_FUEL && (!player->cars[i].boostStreakTimer.isStarted() || BoostStreakTicks > 50)) {
			player->cars[i].streak[player->cars[i].boostStreakCounter].SpawnSprite(this->BoostSprite, &player->cars[i]);

			player->cars[i].boostStreakCounter++;
			if (player->cars[i].boostStreakCounter > 4)
				player->cars[i].boostStreakCounter = 0;

			if (BoostStreakTicks > 50)
				player->cars[i].boostStreakTimer.stop();
			player->cars[i].boostStreakTimer.start();
		}
	}
}