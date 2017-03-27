#pragma once
#include "SDL.h"
class Window
{
private:
	SDL_GLContext context;
	SDL_Window* window;
	const char* title;
	int width, height;
public:
	Window(const char* _title, int _width, int _height, bool _lockMouse);
	Window();
	void InitWindow(bool _lockMouse);
	void Refresh();
	~Window();
};

