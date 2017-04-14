// System
#include <stdio.h>

// Dependencies
#include <SDL.h>
#include <SDL_video.h>
#include <GL\glew.h>
#include <SDL_ttf.h>
#include <glm.hpp>

// Engine
#include "GameObject.h"
#include "InputManager.h"
#include "OpenGLRenderer.h"
#include "Clock.h"
#include "Model.h"
#include "FileReader.h"
#include "FPS_Camera.h"
#include "Window.h"
#include "BoxCollider.h"
#include "Rigidbody.h"
#include "Light.h"


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

InputManager* inputManager = InputManager::GetInstance();
int main(int argc, char** argv) 
{		
	window = new Window(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, true);

	glm::vec3 camRotation = glm::vec3(0.0f);	

	OpenGLRenderer renderer;
	Clock clock;

	FPS_Camera* fpsCamera = new FPS_Camera();
	fpsCamera->SetupProjection(120.0f, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);
	fpsCamera->SetPosition(glm::vec3(0.0f, 0.0f, -20.0f));	

	Shader modelShader = Shader(FileReader::ReadFromFile("Shaders/modelShaderVertex.glsl").c_str(), FileReader::ReadFromFile("Shaders/modelShaderFrag.glsl").c_str());

	GameObject* skull = new GameObject();
	skull->transform->SetPosition(glm::vec3(0.0, 0.0f, 0.0f));

	GameObject* cube = new GameObject();
	cube->transform->SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));

	skull->AddComponent(new Model("Models/nanosuit/nanosuit.obj", modelShader, fpsCamera, skull->transform));
	Model* model = skull->GetComponent<Model>();

	skull->transform->SetPosition(glm::vec3(0.0f, -5.0f, 10.0f));

	skull->AddComponent(new BoxCollider(skull->transform));
	BoxCollider* skullCol = skull->GetComponent<BoxCollider>();
	
	cube->AddComponent(new Model("Models/cube.obj", modelShader, fpsCamera, cube->transform));
	Model* cubeModel = cube->GetComponent<Model>();		


	cube->AddComponent(new Rigidbody(cube->transform));
	Rigidbody* cubeRb = cube->GetComponent<Rigidbody>();
	cubeRb->velocity = glm::vec3(0.5f, 0.0f, 0.0f);

	cube->AddComponent(new BoxCollider(cube->transform));
	BoxCollider* cubeCol = cube->GetComponent<BoxCollider>();


	Light sunLight = Light(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f, 0.67f, 1.0f),
		glm::vec3(0.25f));

	sunLight.direction = glm::vec4(-0.2f, -1.0f, -0.3f, 1.0f); 

	Light pointLight1 = Light(glm::vec3(0.0f, 20.0f, 20.0f), glm::vec3(10.0f), glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(0.1f));
	pointLight1.constant = 1.0f;
	pointLight1.linear = 0.07f;
	pointLight1.quadratic = 0.032f;

	Light pointLight2 = Light(glm::vec3(5.0f, 20.0f, 20.0f), glm::vec3(10.0f), glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(0.1f));
	pointLight2.constant = 1.0f;
	pointLight2.linear = 0.09f;
	pointLight2.quadratic = 0.032f;

	std::vector<Light> lights;
	lights.push_back(pointLight1);
	lights.push_back(pointLight2);


	renderer.Enable();

	SDL_Event windowEvent;
	Sint32 mouseX = 0.0f;
	Sint32 mouseY = 0.0f;

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
				mouseX = windowEvent.motion.xrel;
				mouseY = windowEvent.motion.yrel;

				camRotation.x += (mouseX * 40.0f * clock.GetDeltaTime()) * -1.0f;
				camRotation.y += mouseY * 40.0f * clock.GetDeltaTime();
			}			
		}
		//

		// Game loop
		renderer.PreRender();
		renderer.Render();

		fpsCamera->Update();
		fpsCamera->Rotate(camRotation.x, camRotation.y, camRotation.z);

		model->Render(sunLight,lights);
		skull->transform->Rotate(glm::vec3(0.0f, clock.GetDeltaTime(), 0.0f));

		cubeModel->Render(sunLight,lights);
		

		skull->Update((float)clock.GetDeltaTime());
		cube->Update((float)clock.GetDeltaTime());


		if (cubeCol->IsIntersectingBox(*skullCol))
		{
			cubeRb->velocity.x = -cubeRb->velocity.x;
		}

		if (inputManager->IsKeyDown(SDLK_w)) {
			fpsCamera->Walk(clock.GetDeltaTime() * 50.0f);
		}
		else if (inputManager->IsKeyDown(SDLK_s)) {
			fpsCamera->Walk(-(clock.GetDeltaTime() * 50.0f));
		}

		if (inputManager->IsKeyDown(SDLK_a)) {
			fpsCamera->Strafe(-clock.GetDeltaTime() * 50.0f);
		}
		else if (inputManager->IsKeyDown(SDLK_d)) {
			fpsCamera->Strafe(clock.GetDeltaTime() * 50.0f);
		}

		if (inputManager->IsKeyDown(SDLK_f)) {
			lights[0].position.z -= 10.0f * clock.GetDeltaTime();
			lights[1].position.z -= 10.0f * clock.GetDeltaTime();
		}



		renderer.PostRender();
		//

		window->Refresh();
		
	}

	quitGame();
	
	return 0;
}