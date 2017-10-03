#include "../../Objects.h"

void SceneManager::Init(Assets* assets, Uint32* ts, Player* p) {
	this->timeStep = ts;
	this->player = p;
	this->titleScreen.Init(assets, this);
	this->carSelection.Init(assets, this);
	this->gameOver.Init(assets, this);
	this->credits.Init(assets, this);

	this->current = Scene_TitleScreen;
	this->titleScreen.SceneStart();
}

void SceneManager::StartScene(Scenes scene) {
	this->current = scene;
	switch (this->current) {
	case Scene_TitleScreen:
		this->titleScreen.SceneStart();
		break;
	case Scene_CarSelection:
		this->carSelection.SceneStart();
		break;
	case Scene_GameOver:
		this->gameOver.SceneStart();
		break;
	case Scene_Credits:
		this->credits.SceneStart();
		break;
	default:
		break;
	}
}

Scenes SceneManager::GetScene() {
	return this->current;
}

bool SceneManager::IsScene(Scenes scene) {
	return this->current == scene;
}

void SceneManager::Update() {
	switch (this->current) {
	case Scene_TitleScreen:
		this->titleScreen.Update();
		break;
	case Scene_CarSelection:
		this->carSelection.Update();
		break;
	case Scene_GameOver:
		this->gameOver.Update();
		break;
	case Scene_Credits:
		this->credits.Update();
		break;
	default:
		break;
	}
}