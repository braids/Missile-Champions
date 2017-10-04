#include "../../Objects.h"

void SceneManager::Init(Assets* assets, Uint32* ts, Player* p) {
	this->timeStep = ts;
	this->player = p;
	this->sceneList.push_back(dynamic_cast<TitleScreen*>(&this->titleScreen));
	this->sceneList.push_back(&this->carSelection);
	this->sceneList.push_back(&this->carSelection); // Remove this! It's a placeholder until the Gameplay scene is in.
	//this->sceneList.push_back(&this->gameplay);
	this->sceneList.push_back(&this->gameOver);
	this->sceneList.push_back(&this->credits);

	for (int i = 0; i < (int) this->sceneList.size(); i++) {
		if (i == (int) Scene_Gameplay) continue;	// Remove this! It's a placeholder until the Gameplay scene is in.
		this->sceneList[i]->Init(assets, this);
	}

	this->StartScene(Scene_TitleScreen);
}

void SceneManager::StartScene(Scenes scene) {
	this->current = scene;
	if(this->current != Scene_Gameplay)				// Remove this! It's a placeholder until the Gameplay scene is in.
		this->sceneList[this->current]->SceneStart();
}

Scenes SceneManager::GetScene() {
	return this->current;
}

bool SceneManager::IsScene(Scenes scene) {
	return this->current == scene;
}

void SceneManager::Update() {
	if (this->current != Scene_Gameplay)			// Remove this! It's a placeholder until the Gameplay scene is in.
		this->sceneList[this->current]->Update();
}