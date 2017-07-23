#ifndef _TIMER_H_
#define _TIMER_H_

#include <SDL2/SDL.h>

class Timer {
private:
	Uint32 mStartTicks;
	Uint32 mPausedTicks;

	bool mPaused;
	bool mStarted;

public:
	Timer();

	void start();
	void stop();
	void pause();
	void unpause();

	Uint32 getTicks();

	bool isStarted();
	bool isPaused();
};

#endif