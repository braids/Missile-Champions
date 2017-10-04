#ifndef _CAR_H_
#define _CAR_H_

#include <random>
#include <vector>
#include "Assets.h"
#include "Timer.h"

// Boost defs
#define MAX_BOOST_FUEL 10000
#define MIN_BOOST_FUEL 0

// Cursor defs
#define CURSOR_P1_DEFAULT_X 16
#define CURSOR_P1_DEFAULT_Y 64
#define CURSOR_P2_DEFAULT_X 128
#define CURSOR_P2_DEFAULT_Y 64
#define CURSOR_MOVE_X 48
#define CURSOR_MOVE_Y 32
#define CURSOR_ROW_MIN 0
#define CURSOR_ROW_MAX 3
#define CURSOR_COL_MIN 0
#define CURSOR_COL_MAX 1

// Forward decs
struct Ball;
struct BoostStreak;
struct Car;
struct Player;
struct Camera;
struct Cursor;
struct Scene;
struct SceneManager;


struct BoostStreak {
	Car* parent;
	Assets::Image* image;
	SDL_Rect* viewportRect;
	int angleSprite, decaySprite;
	double x, y, z;
	int timeAlive;

	void SpawnSprite(Assets::Image* sprite, Car* car);
	void UpdateDecaySprite(Uint32 timestep);
	void UpdateViewport(Camera* camera);
};

struct Car {
	Player* parent;
	Assets* assets;
	Assets::Image* image;
	SDL_Rect* viewportRect;
	int anglesprite;
	double x, y, z;
	double dx, dy, dz;
	double angle, speed;
	bool ballCollide;
	bool isBoosting;
	bool isJumping;
	int boostStreakCounter;
	int boostFuel;
	enum Movement {
		NoMovement,
		Forward,
		Backward
	} MoveDirection;

	enum Turn {
		NoTurning,
		Left,
		Right
	} Turning;
	Timer boostStreakTimer;
	Timer boostRechargeTimer;
	Timer jumpTimer;
	BoostStreak streak[5];
	
	void InitCar(Assets* a, Player* player);
	void SetCarSelect();
	void SetCarKickoff(int car);
	void UpdateViewport(Camera* camera);
	double cx();
	double cy();
	double cz();
};

struct Ball {
	Assets::Image* sprites;
	Assets::Image* image;
	SDL_Rect* viewportRect;
	int frame;
	double x, y, z;
	double dx, dy, dz;
	double speed;
	Timer ballAnimate;
	void updatePosition(Uint32 ts);
	void initBall(Assets::Image* a);
	void resetBall();
	void UpdateViewport(Camera* camera);
	double cx();
	double cy();
	double cz();
};

struct Player {
	int		player_num;
	int		team;
	int		score;
	SDL_Rect* selectionViewport;
	Car*	activeCar;
	Car		cars[3];
	void Init(Assets* assets, int p_num);
	void SetCarSelection();
	void SetStartRound();
	void SetKickoff();
};

struct Camera {
	Assets::Image* drawarea;
	Assets::Image* viewport;
	double x, y;
	double dx, dy;
	void Init(Assets::Image* fielddraw, Assets::Image* fieldview);
	void CenterOnCar(Car* activeCar);
};

struct Cursor {
	Assets::Image* image;
	int column, row;
	int defaultX, defaultY;
	enum Event {
		SelectUp = 1 << 0,
		SelectDown = 1 << 1,
		SelectLeft = 1 << 2,
		SelectRight = 1 << 3,
		NoSelection = 0
	} SelectEvent;
	friend Event operator |(Event a, Event b) { return (Event)((int)a | (int)b); }
	friend Event& operator |=(Event& a, Event b) { return a = a | b; }
	friend Event operator &(Event a, Event b) { return (Event)((int)a & (int)b); }
	friend Event& operator &=(Event& a, Event b) { return a = a & b; }

	void Init(Assets::Image* cursor);
	void SetP1();
	void SetP2();
	void MoveCursor();
	int GetSelection();
};

struct Scene {
	// SceneManager pointer
	SceneManager*	parent;

	// Scene assets
	Assets::Image*	BG;
	Mix_Music*	MusicBG;
	Timer	MusicTimer;

	// Scene methods
	virtual void Init(Assets* assets, SceneManager* sceneManager) = 0;
	virtual Assets::Image* GetBG() = 0;
	virtual void SceneStart() = 0;
	virtual void Update() = 0;
};

enum Scenes {
	Scene_TitleScreen,
	Scene_CarSelection,
	Scene_Gameplay,
	Scene_GameOver,
	Scene_Credits
};

struct SceneManager {
	Scenes current;
	Uint32* timeStep;
	Player* player;
	std::vector<Scene*> sceneList;

	struct TitleScreen : public Scene {
		Assets::Image*	bgHidden;
		Assets::Image*	bgVisible;

		Mix_Chunk*	SoundStartSelection;

		bool visible;
		
		struct {
			bool StartGame;
		} events;
		
		struct {
			struct {
				Uint32 duration;
				const Uint32 onInterval = 400;
				const Uint32 offInterval = 200;
				const Uint32 stopInterval = 1200;
				const Uint32 endTime = 2000;
			} flash;
		} effects;

		void Init(Assets* assets, SceneManager* sceneManager);
		Assets::Image* GetBG();
		void SceneStart();
		void Update();
		void StartGameEvent();
	} titleScreen;

	struct CarSelection : public Scene {
		Assets::Image*	DefaultBG;
		Assets::Image*	FlashP1;
		Assets::Image*	FlashP2;
		Assets::Image*	CarWindows;

		Mix_Chunk*	SoundMoveCursor;
		Mix_Chunk*	SoundSelection;
		
		Cursor	SelectCursor;
		
		struct {
			bool Select;
			bool SelectP1;
			bool SelectP2;
		} events;

		struct {
			struct {
				Uint32 duration;
				const Uint32 onInterval = 100;
				const Uint32 offInterval = 50;
				const Uint32 stopInterval = 450;
				const Uint32 endTime = 1250;
			} flash;
		} effects;

		void Init(Assets* assets, SceneManager* sceneManager);
		void SceneStart();
		Assets::Image* GetBG();
		void Update();
		void SelectEvent();
		void SelectP1Event();
		void SelectP2Event();
	} carSelection;

	struct Gameplay : public Scene {
		int ActiveCar;

		Ball GameBall;

		// Camera
		Camera GameplayCamera;

		// Images
		Assets::Image*	StatusBar;
		Assets::Image*	FieldBottom;
		Assets::Image*	BoostSprite;
		Assets::Image*	BoostF1Sprite;
		Assets::Image*	BoostF2Sprite;

		//Sounds
		Mix_Chunk* SoundMoveCursor;
		Mix_Chunk* SoundStartSelection;
		Mix_Chunk* SoundEngine;
		Mix_Chunk* SoundBoost;
		Mix_Chunk* SoundBuzzer;
		Mix_Chunk* SoundBallhit;

		// Random generator
		std::default_random_engine randomEngine;

		//Sort this shit out
		SDL_Rect* BallIndicatorRect;
		bool BallOffscreen;
		int BallIndicatorDirection;

		SDL_Rect* P1Score;
		SDL_Rect* P2Score;
		Assets::Image* Numbers;
		SDL_Rect* Minute10sRect;
		SDL_Rect* Minute1sRect;
		SDL_Rect* Second10sRect;
		SDL_Rect* Second1sRect;
		Assets::Image* Countdown321;
		SDL_Rect* Countdown321Rect;
		Assets::Image* CountdownG;
		SDL_Rect* CountdownGRect;
		Assets::Image* CountdownO;
		SDL_Rect* CountdownORect;

		Assets::Image*	BoostBar;
		SDL_Rect*		BoostBarScaleRect;

		Timer	MusicTimer;
		Timer	ShadowBlinkTimer;
		Timer	RoundStartTimer;
		Timer	RoundTimer;
		Uint32	RoundTicks;
		Timer	GoalTimer;

		// Events and effects
		bool Event_ChangeCar;
		bool Event_LeftGoal;
		bool Event_RightGoal;

		void Init(Assets* assets, SceneManager* sceneManager);
		void SceneStart();
		Assets::Image* GetBG();
		void Update();
		void BallUpdate();
		void PlayerCarsUpdate(Player * player);
	} gameplay;
	
	struct GameOver : public Scene {
		Timer WaitTimer;
		const Uint32 WaitTimerEnd = 5000;

		void Init(Assets* assets, SceneManager* sceneManager);
		void SceneStart();
		Assets::Image* GetBG();
		void Update();
	} gameOver;

	struct Credits : public Scene {
		SDL_Rect* BGRect;
		double ScrollY;
		const double ScrollYSpeed = 0.0075;
		const double ScrollYStop = -672.0;

		Timer CreditsTimer;
		const Uint32 CreditsTimerStartScroll = 3000;
		const Uint32 CreditsTimerEnd = 102000;

		void Init(Assets* assets, SceneManager* sceneManager);
		void SceneStart();
		Assets::Image* GetBG();
		SDL_Rect* GetBGRect();
		void Update();
	} credits;

	void Init(Assets* assets, Uint32* ts, Player* p);
	void StartScene(Scenes scene);
	Scenes GetScene();
	bool IsScene(Scenes scene);
	void Update();
};

#endif
