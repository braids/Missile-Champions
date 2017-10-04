#include <cmath>
#include <iostream>
#include "MChamps.h"

void MChamps::OnLoop() {
	// If starting game from title screen
	if (scene.titleScreen.events.StartGame) {
		scene.titleScreen.StartGameEvent();
		
		if (!scene.titleScreen.events.StartGame) {
			CurrentScene = Scene_CarSelection;
			scene.StartScene(Scene_CarSelection);
		}
	}

	// Car selection events
	if (scene.carSelection.events.Select) scene.carSelection.SelectEvent();
	if (scene.carSelection.events.SelectP1) scene.carSelection.SelectP1Event();
	if (scene.carSelection.events.SelectP2) scene.carSelection.SelectP2Event();

	scene.Update();
	CurrentScene = scene.current;
}
