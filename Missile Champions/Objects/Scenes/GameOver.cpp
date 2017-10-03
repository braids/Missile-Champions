#include "../../Objects.h"

void SceneManager::GameOver::Init(Assets* assets, SceneManager* sceneManager) {
	this->parent = sceneManager;
	this->BG = &assets->images.GameOver;
}

void SceneManager::GameOver::SceneStart() {
	this->WaitTimer.stop();
	this->WaitTimer.start();
}

void SceneManager::GameOver::Update() {
	if (this->WaitTimer.getTicks() > this->WaitTimerEnd) {
		this->WaitTimer.stop();
		this->parent->StartScene(Scene_TitleScreen);
	}
}

Assets::Image* SceneManager::GameOver::GetBG() {
	return this->BG;
}