#ifndef _ASSETS_H_
#define _ASSETS_H_

#define BALL_FRAMES 4
#define BALL_INDICATOR_FRAMES 8
#define BOOST_ROT_FRAMES 8
#define CAR_ROT_FRAMES	16
#define CAR_PALETTES	8
#define NUMBER_FRAMES 10

//#define CHANNEL_MUSIC 0
#define CHANNEL_ENGINE 1
#define CHANNEL_BOOST 2
#define CHANNEL_SELECTION 3
#define CHANNEL_CURSOR 4
#define CHANNEL_TITLESTART 5
#define CHANNEL_BUZZER 6
#define CHANNEL_BALLHIT 7

// Ball Indicator defines
#define UP_LEFT 0
#define UP 1
#define UP_RIGHT 2
#define RIGHT 3
#define DOWN_RIGHT 4
#define DOWN 5
#define DOWN_LEFT 6
#define LEFT 7


#include <map>
#include <SDL2/SDL_mixer.h>
#include "Graphics.h"

typedef std::map<std::string, SDL_Texture*> TextureList;

class Assets {
private:
	static Assets* sInstance;
	TextureList	mTextures;
	Assets();
	~Assets();
public:
	static Assets* Instance();
	static void Release();
	SDL_Texture* GetTexture(std::string name);
	Mix_Chunk* GetSound(std::string name);
	Mix_Music* GetMusic(std::string name);

	struct Image {
		SDL_Texture* texture;
		SDL_Rect* rect;
	};

	struct Images {
		// Title Screen
		Image TitleScreenStartVisible;
		Image TitleScreenStartHidden;
		// Car Sprites
		Image CarSprites[CAR_ROT_FRAMES][CAR_PALETTES];
		Image CarShadow;
		// Car Selection
		Image CarSelectBGDefault;
		Image CarSelectBGP1Flash;
		Image CarSelectBGP2Flash;
		//Image CarSelectWindowSheet;
		Image CarSelectWindowSprites[CAR_PALETTES];
		Image CarSelectCursor;
		// Gameplay
		Image BallSprites[BALL_FRAMES];
		Image BallShadow;
		Image BallIndicatorSprites[BALL_INDICATOR_FRAMES];
		Image FieldDrawArea;
		Image FieldBottom;
		Image FieldViewport;
		Image StatusBar;
		Image Numbers[NUMBER_FRAMES];
		Image BoostBar;
		// Boost
		Image BoostSprite[BOOST_ROT_FRAMES];
		Image BoostF1Sprite[BOOST_ROT_FRAMES];
		Image BoostF2Sprite[BOOST_ROT_FRAMES];
		// Credits
		Image Credits;
		// Game Over
		Image GameOver;
	} images;

	struct Sounds {
		Mix_Chunk* StartSelection;
		Mix_Chunk* Selection;
		Mix_Chunk* MoveCursor;
		Mix_Chunk* Engine;
		Mix_Chunk* Boost;
		Mix_Chunk* Buzzer;
		Mix_Chunk* Ballhit;
	} sounds;

	struct Music {
		Mix_Music* Title;
		Mix_Music* CarSelection;
		Mix_Music* Eurobeat;
		Mix_Music* Credits;
	} music;
};

#endif
