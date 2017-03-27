#include "Clock.h"
#include "SDL.h"

Clock::Clock()
{
	prevTime = 0.0;
	currTime = 0.0;

	deltaTime = -1.0;
}

Clock::~Clock()
{

}

double Clock::GetDeltaTime()
{
	return deltaTime;
}

void Clock::Tick()
{
	currTime = SDL_GetTicks();
	deltaTime = (double)(currTime - prevTime) / 1000;
	prevTime = currTime;
}

