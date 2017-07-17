#include "Graphics.h"

Graphics* Graphics::sInstance = NULL;
bool Graphics::sInitialized = false;

Graphics::Graphics() {
	mBackBuffer = NULL;

	sInitialized = Init();
}

Graphics::~Graphics() {
	SDL_DestroyWindow(mWindow);
	mWindow = NULL;

	SDL_DestroyRenderer(mRenderer);
	mRenderer = NULL;

	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

bool Graphics::Init()
{
	// SDL Initialization
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;

	// Create window
	if ((mWindow = SDL_CreateWindow("MISSILE CHAMPIONS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS)) == NULL)
		return false;

	// Create renderer
	if ((mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED)) == NULL)
		return false;

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

	if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
		return false;

	// SDL_TTF Initialization
	if (TTF_Init() < 0)
		return false;

	mBackBuffer = SDL_GetWindowSurface(mWindow);

	return true;
}

SDL_Texture * Graphics::LoadTexture(std::string filePath) {
	SDL_Texture* texture = NULL;

	SDL_Surface* surface = IMG_Load(filePath.c_str());

	if (surface == NULL)
		return NULL;

	texture = SDL_CreateTextureFromSurface(mRenderer, surface);

	if (texture == NULL)
		return NULL;

	SDL_FreeSurface(surface);

	return texture;
}

SDL_Texture * Graphics::LoadText(TTF_Font* font, std::string text, Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_Texture* texture = NULL;

	SDL_Color color;

	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 0;

	SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);

	if (surface == NULL)
		return NULL;

	texture = SDL_CreateTextureFromSurface(mRenderer, surface);

	if (texture == NULL)
		return NULL;

	SDL_FreeSurface(surface);

	return texture;
}

void Graphics::Render() {
	SDL_RenderSetScale(mRenderer, DScaleXRatio(), DScaleYRatio());
	SDL_RenderPresent(mRenderer);
}

Graphics* Graphics::Instance() {
	if (sInstance == NULL)
		sInstance = new Graphics();

	return sInstance;
}

void Graphics::Release() {
	delete sInstance;
	sInstance = NULL;

	sInitialized = false;
}

bool Graphics::Initialized() {
	return sInitialized;
}

TTF_Font* Graphics::LoadFont(std::string filePath, int size) {
	TTF_Font* font = TTF_OpenFont(filePath.c_str(), size);

	if (font == NULL)
		return NULL;

	return font;
}

void Graphics::ClearBackBuffer() {
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mRenderer);
}

void Graphics::ResetDrawColor() {
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
}

SDL_Rect* Graphics::CreateRect(int w, int h, int x, int y) {
	SDL_Rect* rect = new SDL_Rect();

	rect->w = w;
	rect->h = h;
	rect->x = x;
	rect->y = y;

	return rect;
}

SDL_Rect * Graphics::Fullscreen()
{
	SDL_Rect* FullscreenRect = new SDL_Rect();
	
	FullscreenRect->x = 0;
	FullscreenRect->y = 0;
	FullscreenRect->w = Graphics::GAMEAREA_WIDTH;
	FullscreenRect->h = Graphics::GAMEAREA_HEIGHT;
	
	return FullscreenRect;
}

SDL_Rect* Graphics::TextureRectXY(SDL_Texture * texture, int x, int y)
{
	SDL_Rect* rect = new SDL_Rect();

	SDL_QueryTexture(texture, NULL, NULL, &rect->w, &rect->h);
	rect->x = x;
	rect->y = y;

	return rect;
}

SDL_Rect* Graphics::CenterTextureRectX(SDL_Texture * texture, int y)
{
	SDL_Rect* rect = new SDL_Rect();

	SDL_QueryTexture(texture, NULL, NULL, &rect->w, &rect->h);
	rect->x = (GAMEAREA_WIDTH / 2) - (rect->w / 2);
	rect->y = y;

	return rect;
}

void Graphics::DrawTexture(SDL_Texture * texture) {
	SDL_RenderCopy(mRenderer, texture, NULL, NULL);
}

void Graphics::DrawTexture(SDL_Texture * texture, SDL_Rect* rect) {
	SDL_RenderCopy(mRenderer, texture, NULL, rect);
}

void Graphics::DrawTextureAtLocation(SDL_Texture * texture, SDL_Rect* spriterect, SDL_Rect* rect) {
	SDL_RenderCopy(mRenderer, texture, spriterect, rect);
}

void Graphics::DrawLine(int r, int g, int b, int x1, int y1, int x2, int y2) {
	SDL_SetRenderDrawColor(mRenderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawLine(mRenderer, x1, y1, x2, y2);
	ResetDrawColor();
}
