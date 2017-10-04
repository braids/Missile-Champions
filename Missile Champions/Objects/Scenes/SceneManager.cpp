#include "../../Objects.h"

void SceneManager::Init(Assets* assets, Uint32* ts, Player* p) {
	this->timeStep = ts;
	this->player = p;
	this->sceneList.push_back(&this->titleScreen);
	this->sceneList.push_back(&this->carSelection);
	this->sceneList.push_back(&this->gameplay);
	this->sceneList.push_back(&this->gameOver);
	this->sceneList.push_back(&this->credits);

	for (int i = 0; i < (int) this->sceneList.size(); i++) 
		this->sceneList[i]->Init(assets, this);

	this->StartScene(Scene_TitleScreen);
}

void SceneManager::StartScene(Scenes scene) {
	this->sceneList[scene]->SceneStart();
	this->current = scene;
	this->Update();
}

Scenes SceneManager::GetScene() {
	return this->current;
}

bool SceneManager::IsScene(Scenes scene) {
	return this->current == scene;
}

void SceneManager::Update() {
	this->sceneList[this->current]->Update();
}