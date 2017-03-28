#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <memory>
#include "SDL.h"
class InputManager
{
private:
	InputManager() {};
	~InputManager() {};
	static std::unique_ptr<InputManager> instance;
	friend std::default_delete<InputManager>;
public:
	SDL_KeyboardEvent keyboardEvent;
	static InputManager* GetInstance();
	bool IsKeyDown(SDL_Keycode key);

	void SetKeyEvent(SDL_KeyboardEvent _key);

};
#endif