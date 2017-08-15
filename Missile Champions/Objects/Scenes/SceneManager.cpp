#include "../../Objects.h"

void SceneManager::Init(Assets* assets, Uint32* ts) {
	this->timeStep = ts;
	this->titleScreen.Init(assets, this);
}

Scenes SceneManager::GetScene() {
	return this->current;
}

bool SceneManager::IsScene(Scenes scene) {
	return this->current == scene;
}