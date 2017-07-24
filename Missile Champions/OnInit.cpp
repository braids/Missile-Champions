#include "MChamps.h"
#include "AssetDefines.h"

bool MChamps::OnInit() {
	printf("Init\n");

	// Initialize graphics
	mGraphics = Graphics::Instance();

	// Initialize assets
	mAssets = Assets::Instance();

	//// Initialize art assets
	// Title Screen
	mAssets->images.TitleScreenStartHidden = { Assets::Instance()->GetTexture(IMAGE_TITLE_START_HIDDEN), Graphics::Fullscreen() };
	mAssets->images.TitleScreenStartVisible = { Assets::Instance()->GetTexture(IMAGE_TITLE), Graphics::Fullscreen() };
	TitleScreenBG = &mAssets->images.TitleScreenStartVisible;
	// Car Sprites
	for (int i = 0, j = 0; j < CAR_PALETTES;) {
		mAssets->images.CarSprites[i++][j] = { Assets::Instance()->GetTexture(IMAGE_CAR_SPRITE_SHEET), Graphics::CreateRect(32, 32, 32 * i, 32 * j) };
		if (i == CAR_ROT_FRAMES) { i = 0; j++; }
	}
	// Car Selection
	mAssets->images.CarSelectBGDefault = { Assets::Instance()->GetTexture(IMAGE_CAR_SELECT_BG), Graphics::Fullscreen() };
	mAssets->images.CarSelectBGP1Flash = { Assets::Instance()->GetTexture(IMAGE_CAR_SELECT_P1_FLASH), Graphics::Fullscreen() };
	mAssets->images.CarSelectBGP2Flash = { Assets::Instance()->GetTexture(IMAGE_CAR_SELECT_P2_FLASH), Graphics::Fullscreen() };
	CarSelectBG = &mAssets->images.CarSelectBGDefault;
	for (int i = 0; i < CAR_PALETTES; i++)
		mAssets->images.CarSelectWindowSprites[i] = { Assets::Instance()->GetTexture(IMAGE_CAR_SELECTION_SHEET), Graphics::CreateRect(64, 64, 64 * i, 0) };
	mAssets->images.CarSelectCursor = { Assets::Instance()->GetTexture(IMAGE_CAR_SELECTION_CURSOR), Graphics::CreateRect(64, 64, 16, 64) };
	// Gameplay
	for (int i = 0; i < BALL_FRAMES; i++)
		mAssets->images.BallSprites[i] = { Assets::Instance()->GetTexture(IMAGE_FOOTBALL_SPRITE_SHEET), Graphics::CreateRect(48, 48, 48 * i, 0) };
	mAssets->images.FieldDrawArea = { Assets::Instance()->GetTexture(IMAGE_FIELD), Graphics::CreateRect(CAMERA_W, CAMERA_H, 0, 0) };
	mAssets->images.FieldBottom = { Assets::Instance()->GetTexture(IMAGE_FIELD_BOTTOM) };
	FieldBottom = &mAssets->images.FieldBottom;
	mAssets->images.FieldViewport = { NULL, Graphics::CreateRect(CAMERA_W, CAMERA_H, 0, 0) };
	mAssets->images.StatusBar = { Assets::Instance()->GetTexture(IMAGE_STATUS_BAR), Graphics::Fullscreen() };
	StatusBar = &mAssets->images.StatusBar;

	//// Effects
	Effect_StartFlashLength = 0;
	Effect_P1FlashLength = 0;
	Effect_P2FlashLength = 0;

	//// Events
	Event_StartGame = false;
	Event_CarSelected = false;
	Event_P1Selected = false;
	Event_P2Selected = false;

	//// Car Selection Cursor
	CarSelectionCursor = { &mAssets->images.CarSelectCursor, 0, 0 };
	
	//// Camera
	GameplayCamera = { &mAssets->images.FieldDrawArea, &mAssets->images.FieldViewport, 0, 0, 0, 0 };
	// Ensure bottom of field tracks with camera
	FieldBottom->rect = GameplayCamera.drawarea->rect;

	// Ball
	GameBall = {
		mAssets->images.BallSprites,		// Ball sprite sheet
		Graphics::CreateRect(48, 48, 0, 0),	// Init viewport rect
		0,			// Frame
		0, 0, 0,	// x, y, z
		0, 0, 0,	// dx, dy, dz
		0, 0		// vx, vy
	};

	//// Players
	// Player 1 init
	Players[0] = { 0, 0, Graphics::CreateRect(64, 64, 40, 8), &Players[0].cars[0] };
	// Player 1 car init
	Players[0].cars[0] = {
		nullptr, // Image
		Graphics::CreateRect(32, 32, 56, 24),
		0,
		0, 0, 0,
		0, 0, 0,
		0, 0,
		0, 0,
		false,
		false,
		Car::NoMovement, Car::NoTurning};
	Players[0].cars[1] = {
		nullptr, // Image
		Graphics::CreateRect(32, 32, 56, 24),
		0,
		0, 0, 0,
		0, 0, 0,
		0, 0,
		0, 0,
		false,
		false,
		Car::NoMovement, Car::NoTurning };
	Players[0].cars[2] = {
		nullptr, // Image
		Graphics::CreateRect(32, 32, 56, 24),
		0,
		0, 0, 0,
		0, 0, 0,
		0, 0,
		0, 0,
		false,
		false,
		Car::NoMovement, Car::NoTurning };
	// Player 2 init
	Players[1] = { 0, 0, Graphics::CreateRect(64, 64, 152, 8), &Players[1].cars[0] };
	// Player 2 car init
	Players[1].cars[0] = {
		nullptr, // Image
		Graphics::CreateRect(32, 32, 168, 24),
		0,
		0, 0, 0,
		0, 0, 0,
		0, 0,
		0, 0,
		false,
		false,
		Car::NoMovement, Car::NoTurning };
	Players[1].cars[1] = {
		nullptr, // Image
		Graphics::CreateRect(32, 32, 168, 24),
		0,
		0, 0, 0,
		0, 0, 0,
		0, 0,
		0, 0,
		false,
		false,
		Car::NoMovement, Car::NoTurning };
	Players[1].cars[2] = {
		nullptr, // Image
		Graphics::CreateRect(32, 32, 168, 24),
		0,
		0, 0, 0,
		0, 0, 0,
		0, 0,
		0, 0,
		false,
		false,
		Car::NoMovement, Car::NoTurning };

	return true;
}