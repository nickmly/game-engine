#include "InputManager.h"

std::unique_ptr<InputManager> InputManager::instance(nullptr);

InputManager * InputManager::GetInstance()
{
	if (instance.get() == nullptr)
	{
		instance.reset(new InputManager());
	}
	return instance.get();
}

bool InputManager::IsKeyDown(SDL_Keycode key)
{
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_GetScancodeFromKey(key)])
	{
		//printf("key pressed: %s\n", SDL_GetKeyName(key));
		return true;
	}
	return false;
}

void InputManager::SetKeyEvent(SDL_KeyboardEvent _keyEvent)
{
	keyboardEvent = _keyEvent;
}



