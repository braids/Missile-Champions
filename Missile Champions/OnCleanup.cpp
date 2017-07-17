#include "MChamps.h"

void MChamps::OnCleanup() {
	Graphics::Release();
	mGraphics = NULL;

	Assets::Release();
	mAssets = NULL;
}