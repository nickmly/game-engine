// System
#include <stdio.h>

// Dependencies
#include <SDL.h>
#include <SDL_video.h>
#include <GL\glew.h>
#include <SDL_ttf.h>
#include <glm.hpp>

// Engine
#include "OpenGLRenderer.h"
#include "Clock.h"
#include "Model.h"
#include "FileReader.h"
#include "FPS_Camera.h"
#include "Window.h"


const char* TITLE = "Window Title";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

TTF_Font *font;

Window* window;

void initTTF()
{
	TTF_Init();
	

}

void quitGame()
{	
	TTF_Quit();
	window->~Window();
}

void renderText(char* text)
{
	SDL_Color col = { 255,0,0,255 };
	font = TTF_OpenFont("Fonts\\cour.ttf", 16);
	SDL_Surface* surf = TTF_RenderText_Blended(font, text, col);


	Uint8 colors = surf->format->BytesPerPixel;
	GLenum texture_format;
	if (colors == 4) {   // alpha
		if (surf->format->Rmask == 0x000000ff)
			texture_format = GL_RGBA;
		else
			texture_format = GL_BGRA;
	}
	else {             // no alpha
		if (surf->format->Rmask == 0x000000ff)
			texture_format = GL_RGB;
		else
			texture_format = GL_BGR;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, colors, surf->w, surf->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, surf->pixels);
	
	const GLfloat quadVertices[] = { -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f,-1.0f, 0.0f,
		-1.0f,-1.0f, 0.0f
	};

	glVertexPointer(3, GL_FLOAT, 0, quadVertices);
	glDrawArrays(GL_QUADS, 0, 4);
	
	TTF_CloseFont(font);
	SDL_FreeSurface(surf);
}

int main(int argc, char** argv) 
{		
	window = new Window(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, false);

	Model* model;
	OpenGLRenderer renderer;
	Clock clock;
	FPS_Camera* fpsCamera = new FPS_Camera();
	fpsCamera->SetupProjection(120.0f, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);
	fpsCamera->SetPosition(glm::vec3(0.0f, 0.0f, -20.0f));	

	Shader modelShader = Shader(FileReader::ReadFromFile("Shaders/model_shader.vert").c_str(), FileReader::ReadFromFile("Shaders/model_shader.frag").c_str());
	model = new Model("Models/skull.obj", modelShader, fpsCamera);

	model->transform = glm::translate(model->transform, glm::vec3(2.5f, 2.5f, 0.0f));
	renderer.Enable();

	SDL_Event windowEvent;
	float mouseX = 0.0f;
	float mouseY = 0.0f;
	while (true)
	{
		clock.Tick();

		// Input handling
		if (SDL_PollEvent(&windowEvent))
		{
			if (windowEvent.type == SDL_QUIT)
				break;
			if (windowEvent.type == SDL_KEYDOWN)
			{
				if (windowEvent.key.keysym.sym == SDLK_ESCAPE)
					break;
			}
			if (windowEvent.type == SDL_MOUSEMOTION)
			{
				mouseX = windowEvent.motion.x;
				mouseY = windowEvent.motion.y;
			}			
		}


		// Game loop
		renderer.PreRender();
		renderer.Render();

		fpsCamera->Update();
		model->Render(glm::vec3(mouseX, mouseY, -20.0f));
		model->transform = glm::rotate(
			model->transform,
			(float)clock.GetDeltaTime()/2.0f,
			glm::vec3(0.0f, 1.0f, 0.0f));

		renderer.PostRender();
		
		window->Refresh();
	}

	quitGame();
	
	return 0;
}