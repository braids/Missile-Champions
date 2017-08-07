#include "../../Objects.h"

void Scene::TitleScreen::Init(Assets* assets) {
	this->bgHidden = &assets->images.TitleScreenStartHidden;
	this->bgVisible = &assets->images.TitleScreenStartVisible;
	this->visible = true;
}

Assets::Image* Scene::TitleScreen::BG() {
	return (this->visible ? this->bgVisible : this->bgHidden);
}