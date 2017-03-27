#include "Window.h"
#include "GL\glew.h"

Window::Window(const char * _title, int _width, int _height, bool _lockMouse)
{
	title = _title;
	width = _width;
	height = _height;
	

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	if(_lockMouse)
		SDL_SetRelativeMouseMode(SDL_TRUE); // Lock mouse to window

	SDL_Window* window = SDL_CreateWindow(title, 100, 100, width, height, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(window);

	glewExperimental = true;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}

void Window::Refresh()
{
	SDL_GL_SwapWindow(window);
}

Window::~Window()
{
	SDL_GL_DeleteContext(context);
	SDL_Quit();
}
