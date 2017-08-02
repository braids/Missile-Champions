#include "MChamps.h"

void MChamps::OnCleanup() {
	Mix_FreeChunk(mAssets->sounds.Boost);
	Mix_FreeChunk(mAssets->sounds.Engine);
	Mix_FreeChunk(mAssets->sounds.MoveCursor);
	Mix_FreeChunk(mAssets->sounds.Selection);
	Mix_FreeChunk(mAssets->sounds.StartSelection);
	Mix_FreeMusic(mAssets->music.CarSelection);
	Mix_FreeMusic(mAssets->music.Eurobeat);
	Mix_FreeMusic(mAssets->music.Title);
	mAssets->sounds.Boost = NULL;
	mAssets->sounds.Engine = NULL;
	mAssets->sounds.MoveCursor = NULL;
	mAssets->sounds.Selection = NULL;
	mAssets->sounds.StartSelection = NULL;
	mAssets->music.CarSelection = NULL;
	mAssets->music.Eurobeat = NULL;
	mAssets->music.Title = NULL;

	Mix_Quit();
	
	Graphics::Release();
	mGraphics = NULL;

	Assets::Release();
	mAssets = NULL;
}