#include "../../Objects.h"

void SceneManager::CarSelection::Init(Assets* assets, SceneManager* sceneManager) {
	this->parent = sceneManager;
	
	this->DefaultBG = &assets->images.CarSelectBGDefault;
	this->FlashP1 = &assets->images.CarSelectBGP1Flash;
	this->FlashP2 = &assets->images.CarSelectBGP2Flash;
	this->BG = this->DefaultBG;
	this->CarWindows = assets->images.CarSelectWindowSprites;
	
	this->SelectCursor.Init(&assets->images.CarSelectCursor);

	this->events.Select = false;
	this->events.SelectP1 = false;
	this->events.SelectP2 = false;

	this->effects.flash.duration = 0;
}

void SceneManager::CarSelection::SceneStart() {
	this->parent->current = Scenes::Scene_CarSelection;

	this->BG = this->DefaultBG;

	this->SelectCursor.SetP1();

	this->events.Select = false;
	this->events.SelectP1 = false;
	this->events.SelectP2 = false;

	this->effects.flash.duration = 0;

	this->parent->player[0].SetCarSelection();
	this->parent->player[1].SetCarSelection();
}

Assets::Image* SceneManager::CarSelection::GetBG() {
	return this->BG;
}

void SceneManager::CarSelection::SelectEvent() {
	if (this->parent->player[0].team == 0) {
		// Start P1 Selection Event
		this->events.SelectP1 = true;
		this->effects.flash.duration = 0;

		// Set P1 Team from cursor selection
		this->parent->player[0].team = this->SelectCursor.GetSelection();

		// Set P1 active car image for selection window
		this->parent->player[0].activeCar->image = 
			&this->parent->player[0].activeCar->assets->images.CarSprites[this->parent->player[0].activeCar->anglesprite][this->parent->player[0].team - 1];

		// Move cursor to P2 car select
		this->SelectCursor.SetP2();
	}
	else if (this->parent->player[1].team == 0) {
		// Start P2 Selection Event
		this->events.SelectP2 = true;
		this->effects.flash.duration = 0;

		// Set P2 Team from cursor selection
		this->parent->player[1].team = this->SelectCursor.GetSelection();

		// Set P2 active car image for selection window
		this->parent->player[1].activeCar->image =
			&this->parent->player[1].activeCar->assets->images.CarSprites[this->parent->player[1].activeCar->anglesprite][this->parent->player[1].team - 1];
	}

	this->events.Select = false;
}

void SceneManager::CarSelection::SelectP1Event() {
	// Add timestep to flash duration
	this->effects.flash.duration += *this->parent->timeStep;

	// Determine if flash is on or off
	if (this->effects.flash.duration % this->effects.flash.onInterval >= this->effects.flash.offInterval)
		this->BG = this->FlashP1;
	else
		this->BG = this->DefaultBG;

	// End flashing if one second of flashing has elapsed.
	if (this->effects.flash.duration >= this->effects.flash.stopInterval) {
		this->effects.flash.duration = 0;
		this->events.SelectP1 = false;
		this->BG = this->DefaultBG;
	}
}

void SceneManager::CarSelection::SelectP2Event() {
	// Add timestep to flash duration
	this->effects.flash.duration += *this->parent->timeStep;


	// Determine if flash is on or off
	if (this->effects.flash.duration % this->effects.flash.onInterval >= this->effects.flash.offInterval)
		this->BG = this->FlashP2;
	else
		this->BG = this->DefaultBG;

	// Stop flashing if one second of flashing has elapsed.
	if (this->effects.flash.duration >= this->effects.flash.stopInterval) {
		this->BG = this->DefaultBG;
	}

	// Delay for one second before ending event.
	if (this->effects.flash.duration >= this->effects.flash.endTime) {
		//this->effects.flash.duration = 0;
		//this->events.SelectP2 = false;
		this->events.SelectP2 = false;
	}
}