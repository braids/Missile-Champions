#include "../../Objects.h"

void SceneManager::TitleScreen::Init(Assets* assets, SceneManager* sceneManager) {
	this->parent = sceneManager;
	this->bgHidden = &assets->images.TitleScreenStartHidden;
	this->bgVisible = &assets->images.TitleScreenStartVisible;
	this->SoundStartSelection = assets->sounds.StartSelection;
	this->MusicBG = assets->music.Title;
	this->visible = true;

	this->events.StartGame = false;

	this->effects.flash.duration = 0;
}

Assets::Image* SceneManager::TitleScreen::BG() {
	return (this->visible ? this->bgVisible : this->bgHidden);
}

void SceneManager::TitleScreen::SceneStart() {
	this->visible = true;
	this->events.StartGame = false;
	this->effects.flash.duration = 0;
	
	Mix_PlayMusic(this->MusicBG, -1);
	this->MusicTimer.stop();
	this->MusicTimer.start();
}

void SceneManager::TitleScreen::Update() {
	// Loop music (because SDL_mixer doesn't seamlessly loop)
	if ((this->MusicTimer.getTicks() > 6410 || Mix_PlayingMusic() == 0) && !this->events.StartGame) {
		Mix_PlayMusic(this->MusicBG, -1);
		this->MusicTimer.stop();
		this->MusicTimer.start();
	}
}

void SceneManager::TitleScreen::StartGameEvent() {
	this->effects.flash.duration += *this->parent->timeStep;

	if (this->effects.flash.duration % this->effects.flash.onInterval > this->effects.flash.offInterval ||
		this->effects.flash.duration >= this->effects.flash.stopInterval)
		this->visible = true;
	else
		this->visible = false;

	if (this->effects.flash.duration >= this->effects.flash.endTime) {
		this->effects.flash.duration = 0;
		this->events.StartGame = false;
	}
}