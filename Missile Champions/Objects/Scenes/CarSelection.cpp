#include "../../Objects.h"

void SceneManager::CarSelection::Init(Assets* assets, SceneManager* sceneManager) {
	this->parent = sceneManager;
	
	this->DefaultBG = &assets->images.CarSelectBGDefault;
	this->FlashP1 = &assets->images.CarSelectBGP1Flash;
	this->FlashP2 = &assets->images.CarSelectBGP2Flash;
	this->BG = DefaultBG;
	this->CarWindows = assets->images.CarSelectWindowSprites;
	
	this->SelectCursor.Init(&assets->images.CarSelectCursor);

	this->events.Select = false;
	this->events.SelectP1 = false;
	this->events.SelectP2 = false;

	this->effects.flash.duration = 0;
}

Assets::Image* SceneManager::CarSelection::GetBG() {
	return this->BG;
}
