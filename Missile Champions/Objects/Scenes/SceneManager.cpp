#include "../../Objects.h"

void SceneManager::Init(Assets* assets, Uint32* ts, Player* p) {
	this->timeStep = ts;
	this->player = p;
	this->titleScreen.Init(assets, this);
}

Scenes SceneManager::GetScene() {
	return this->current;
}

bool SceneManager::IsScene(Scenes scene) {
	return this->current == scene;
}