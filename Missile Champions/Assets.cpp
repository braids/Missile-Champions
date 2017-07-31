#include "Assets.h"

// Assets instance
Assets* Assets::sInstance = NULL;

// Assets instance ctor
Assets* Assets::Instance() {
	if (sInstance == NULL)
		sInstance = new Assets();

	return sInstance;
}

// Assets cleanup
void Assets::Release() {
	delete sInstance;
	sInstance = NULL;
}

// Assets ctor
Assets::Assets() {

}

// Assets dtor. Frees all texture memory.
Assets::~Assets() {
	for (auto tex : mTextures) {
		if (tex.second != NULL)
			SDL_DestroyTexture(tex.second);
	}

	mTextures.clear();
}

// Load texture from file if not in texture map, then return pointer to texture in texture map.
SDL_Texture* Assets::GetTexture(std::string name) {
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("res/img/" + name);

	if (mTextures[fullPath] == NULL)
		mTextures[fullPath] = Graphics::Instance()->LoadTexture(fullPath);

	return mTextures[fullPath];
}

Mix_Chunk* Assets::GetSound(std::string name) {
	std::string fullPath = SDL_GetBasePath();
	fullPath.append("res/snd/" + name);

	return Mix_LoadWAV(fullPath.c_str());
}