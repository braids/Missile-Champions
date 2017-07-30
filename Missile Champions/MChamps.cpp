#include "MChamps.h"

MChamps::MChamps() {
	// Set window and renderer to null
	mGraphics = NULL;
	mAssets = NULL;
	// Set game running/playing flags
	Running = true;
	CurrentScene = Scene_TitleScreen;
	// Set timer values
	currTick = 0;
	lastTick = 0;
}

int MChamps::OnExecute() {
	// Initialize everything
	if (OnInit() == false) {
		return -1;
	}

	SDL_Event Event;

	// Main loop
	while (Running) {
		// Check events
		while (SDL_PollEvent(&Event)) {
			OnEvent(&Event);
		}
		// Perform actions and updates
		OnLoop();
		// Render scene
		OnRender();
	}

	// De-initialize SDL and SDL_TTF
	OnCleanup();

	return 0;
}

int main(int argc, char* argv[]) {
	MChamps mc;

	return mc.OnExecute();
}
