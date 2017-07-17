#include "GameObject.h"

// GameObject generic Ctor. Don't use this!
GameObject::GameObject() {
	Name = NULL;
	//Texture = NULL;
	//DrawArea = NULL;
	posX = 0;
	posY = 0;
	velX = 0;
	velY = 0;
}

// GameObject Ctor. Makes an object at a location with a texture.
GameObject::GameObject(const char* name, SDL_Texture* texture, int x, int y) {
	Name = name;
	//Texture = texture;
	posX = x;
	posY = y;
	velX = 0;
	velY = 1;
	//DrawArea = new SDL_Rect();
}

// Accessors
const char* GameObject::GetName() {
	return Name;
}

/*
SDL_Texture* GameObject::GetTexture() {
	return Texture;
}

SDL_Rect* GameObject::GetDrawArea() {
	// Update draw area width/height from texture
	SDL_QueryTexture(Texture, NULL, NULL, &DrawArea->w, &DrawArea->h);
	// Update draw area X/Y from object position
	DrawArea->column = posX;
	DrawArea->row = posY;
	// Return draw area
	return DrawArea;
}*/