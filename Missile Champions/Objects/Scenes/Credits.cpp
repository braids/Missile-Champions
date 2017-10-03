#include "../../Objects.h"

void SceneManager::Credits::Init(Assets* assets, SceneManager* sceneManager) {
	this->parent = sceneManager;
	this->BG = &assets->images.Credits;
	this->BGRect = Graphics::CreateRect(256, 896, 0, 0);
	this->MusicBG = assets->music.Credits;
}

void SceneManager::Credits::SceneStart() {
	this->BGRect->y = 0;
	this->ScrollY = 0.0;
	
	this->MusicTimer.stop();
	this->MusicTimer.start();
	Mix_PlayMusic(this->MusicBG, 0);

	this->CreditsTimer.stop();
	this->CreditsTimer.start();
}

void SceneManager::Credits::Update() {
	Uint32 ticks = this->CreditsTimer.getTicks();

	if (ticks > this->CreditsTimerStartScroll && this->ScrollY > this->ScrollYStop) {
		this->ScrollY -= this->ScrollYSpeed * (double) *(this->parent->timeStep);
		this->BGRect->y = (int)this->ScrollY;
	}

	if (ticks > this->CreditsTimerEnd) {
		this->MusicTimer.stop();
		this->CreditsTimer.stop();
		this->parent->StartScene(Scene_TitleScreen);
	}
}

Assets::Image* SceneManager::Credits::GetBG() {
	return this->BG;
}

SDL_Rect* SceneManager::Credits::GetBGRect() {
	return this->BGRect;
}