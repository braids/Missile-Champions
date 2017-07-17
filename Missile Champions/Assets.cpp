#include "Assets.h"

Assets* Assets::sInstance = NULL;

Assets* Assets::Instance() {
	if (sInstance == NULL)
		sInstance = new Assets();

	return sInstance;
}

void Assets::Release() {
	delete sInstance;
	sInstance = NULL;
}

Assets::Assets() {

}

Assets::~Assets() {
	for (auto tex : mTextures) {
		if (tex.second != NULL)
			SDL_DestroyTexture(tex.second);
	}

	mTextures.clear();
}

SDL_Texture* Assets::GetTexture(std::string name) {
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("res/img/" + name);

	if (mTextures[fullPath] == NULL)
		mTextures[fullPath] = Graphics::Instance()->LoadTexture(fullPath);

	return mTextures[fullPath];
}