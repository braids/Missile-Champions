#include "../../Objects.h"

void SceneManager::TitleScreen::Init(Assets* assets) {
	this->bgHidden = &assets->images.TitleScreenStartHidden;
	this->bgVisible = &assets->images.TitleScreenStartVisible;
	this->visible = true;

	this->events.StartGame = false;

	this->effects.flash.duration = 0;
}

Assets::Image* SceneManager::TitleScreen::BG() {
	return (this->visible ? this->bgVisible : this->bgHidden);
}

void SceneManager::TitleScreen::StartGameEvent(int timeStep) {
	this->effects.flash.duration += timeStep;

	if (this->effects.flash.duration % this->effects.flash.onInterval > this->effects.flash.offInterval ||
		this->effects.flash.duration >= this->effects.flash.stopInterval)
		this->visible = true;
	else
		this->visible = false;

	if (this->effects.flash.duration >= 2000) {
		this->effects.flash.duration = 0;
		this->events.StartGame = false;
	}
}