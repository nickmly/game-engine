#pragma once
#include "InputManager.h"
#include "Clock.h"

class AbstractScene
{
public:
	//Construct Scene
	virtual void OnCreate() = 0;
	//Initialize Scene
	virtual void OnStart() = 0;
	//Update Scene
	virtual void OnUpdate(float deltaTime) = 0;
	//Render Scene
	virtual void OnRender() = 0;
	//End Scene
	virtual void OnEnd() = 0;

	//Handle Quit
	virtual void OnQuit() = 0;

	//Check if Scene has started.
	virtual bool HasStarted() = 0;
	//Check if Scene has ended.
	virtual bool HasEnded() = 0;

protected:
	InputManager *inputManager = 0;
	Clock *clock = 0;

private:
	bool hasStarted = 0;
	bool hasEnded = 0;
};

