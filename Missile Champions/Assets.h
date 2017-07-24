#ifndef _ASSETS_H_
#define _ASSETS_H_

#define BALL_FRAMES 4
#define BOOST_ROT_FRAMES 5
#define CAR_ROT_FRAMES	4
#define CAR_PALETTES	8

#include <map>
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
		// Car Selection
		Image CarSelectBGDefault;
		Image CarSelectBGP1Flash;
		Image CarSelectBGP2Flash;
		//Image CarSelectWindowSheet;
		Image CarSelectWindowSprites[CAR_PALETTES];
		Image CarSelectCursor;
		// Gameplay
		Image BallSprites[BALL_FRAMES];
		Image FieldDrawArea;
		Image FieldBottom;
		Image FieldViewport;
		Image StatusBar;
		// Boost
		Image BoostSprite[BOOST_ROT_FRAMES];
		Image BoostF1Sprite[BOOST_ROT_FRAMES];
		Image BoostF2Sprite[BOOST_ROT_FRAMES];
	} images;
};

#endif
