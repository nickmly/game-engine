#pragma once

class Clock
{
private:
	int prevTime, currTime;
	double deltaTime;
public:
	Clock();
	~Clock();
	double GetDeltaTime();
	void Tick();
};

