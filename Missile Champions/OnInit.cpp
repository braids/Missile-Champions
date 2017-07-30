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
	mAssets->images.CarShadow = { Assets::Instance()->GetTexture(IMAGE_CAR_SHADOW), Graphics::CreateRect(32, 32, 0, 0) };
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
	mAssets->images.BallShadow = { Assets::Instance()->GetTexture(IMAGE_FOOTBALL_SHADOW), Graphics::CreateRect(48, 48, 0, 0) };
	mAssets->images.FieldDrawArea = { Assets::Instance()->GetTexture(IMAGE_FIELD), Graphics::CreateRect(CAMERA_W, CAMERA_H, 0, 0) };
	mAssets->images.FieldBottom = { Assets::Instance()->GetTexture(IMAGE_FIELD_BOTTOM) };
	FieldBottom = &mAssets->images.FieldBottom;
	mAssets->images.FieldViewport = { NULL, Graphics::CreateRect(CAMERA_W, CAMERA_H, 0, 0) };
	mAssets->images.StatusBar = { Assets::Instance()->GetTexture(IMAGE_STATUS_BAR), Graphics::Fullscreen() };
	StatusBar = &mAssets->images.StatusBar;
	for (int i = 0; i < NUMBER_FRAMES; i++)
		mAssets->images.Numbers[i] = { Assets::Instance()->GetTexture(IMAGE_NUMBERS), Graphics::CreateRect(8, 8, 8 * i, 0) };
	P1Score = Graphics::CreateRect(8, 8, 40, 192);
	P2Score = Graphics::CreateRect(8, 8, 40, 208);
	mAssets->images.BoostBar = { Assets::Instance()->GetTexture(IMAGE_BOOST_BAR), Graphics::CreateRect(1, 8, 0, 0) };
	BoostBar = &mAssets->images.BoostBar;
	BoostBarScaleRect = Graphics::CreateRect(64, 8, 96, 200);
	// Boost Streak
	for (int i = 0; i < BOOST_ROT_FRAMES; i++) {
		mAssets->images.BoostSprite[i] = { Assets::Instance()->GetTexture(IMAGE_BOOST_SPRITE_SHEET), Graphics::CreateRect(32, 32, 32 * i, 0) };
		mAssets->images.BoostF1Sprite[i] = { Assets::Instance()->GetTexture(IMAGE_BOOST_F1_SPRITE_SHEET), Graphics::CreateRect(32, 32, 32 * i, 0) };
		mAssets->images.BoostF2Sprite[i] = { Assets::Instance()->GetTexture(IMAGE_BOOST_F2_SPRITE_SHEET), Graphics::CreateRect(32, 32, 32 * i, 0) };
	}

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
	GameBall.initBall(mAssets->images.BallSprites);

	//// Players
	// Player 1 init
	Players[0] = { 0, 0, Graphics::CreateRect(64, 64, 40, 8), &Players[0].cars[0] };
	// Player 2 init
	Players[1] = { 0, 0, Graphics::CreateRect(64, 64, 152, 8), &Players[1].cars[0] };
	
	// Car init
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 3; j++) {
			Players[i].cars[j].InitCar(mAssets);
		}
	}

	return true;
}