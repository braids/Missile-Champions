#include "MChamps.h"

void MChamps::OnCleanup() {
	Mix_Quit();
	
	Graphics::Release();
	mGraphics = NULL;

	Assets::Release();
	mAssets = NULL;
}