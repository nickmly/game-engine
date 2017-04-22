// System
#include <stdio.h>

// Dependencies
#include <SDL.h>
#include <SDL_video.h>
#include <GL\glew.h>
#include <SDL_ttf.h>
#include <glm.hpp>

// Bullet Physics
#include <btBulletDynamicsCommon.h>
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>

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
#include "PhysicsWorld.h"
#include "BulletRigidbody.h"
#include "Skybox.h"


const char* TITLE = "Window Title";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int NUM_OF_BOXES = 8;

TTF_Font *font;

Window* window;


// Bullet
PhysicsWorld* world;
btCollisionShape* groundShape;
btCollisionShape* fallShape;

btDefaultMotionState* groundMotionState;
btDefaultMotionState* fallState;

BulletRigidbody* groundRb;
BulletRigidbody* fallRb;

Skybox* skybox;

void initTTF()
{
	TTF_Init();


}


void initPhysicsSim()
{
	world = new PhysicsWorld();

	groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	fallShape = new btBoxShape(btVector3(1.25, 1.25, 1.25));

	groundRb = new BulletRigidbody(groundShape, btVector3(0, -1, 0), btQuaternion(0, 0, 0, 1), 0.0f);
	world->AddRigidbody(groundRb->GetBtRigidbody());
}

void quitGame()
{
	TTF_Quit();
	window->~Window();
	delete world;
	delete groundShape;
	delete fallShape;

	delete groundMotionState;
	delete fallState;

	delete groundRb;
	delete fallRb;
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
	bool focusOnMouse = true;
	initPhysicsSim();

	window = new Window(TITLE, SCREEN_WIDTH, SCREEN_HEIGHT, true);

	glm::vec3 camRotation = glm::vec3(0.0f);

	OpenGLRenderer renderer;
	Clock clock;

	FPS_Camera* fpsCamera = new FPS_Camera();
	fpsCamera->SetupProjection(120.0f, SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 1000.0f);
	fpsCamera->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	Shader modelShader = Shader(FileReader::ReadFromFile("Shaders/modelShaderVertex.glsl").c_str(), FileReader::ReadFromFile("Shaders/modelShaderFrag_Refraction.glsl").c_str());
	Shader lampShader = Shader(FileReader::ReadFromFile("Shaders/modelShaderVertex.glsl").c_str(), FileReader::ReadFromFile("Shaders/modelShaderFrag_NoLight.glsl").c_str());
	GameObject* skull = new GameObject();
	skull->transform->SetPosition(glm::vec3(0.0, 0.0f, 0.0f));


	GameObject* cubes[NUM_OF_BOXES];
	for (int i = 0; i < NUM_OF_BOXES; i++)
	{
		cubes[i] = new GameObject();
		cubes[i]->AddComponent(new Model("Models/cube.obj", modelShader, fpsCamera, cubes[i]->transform));
		//Model* cubeModel = cubes[i]->GetComponent<Model>();
		fallRb = new BulletRigidbody(fallShape, btVector3(0, 50 + (i * 2), 0), btQuaternion(0, 0, 0, 1), 1.0f);
		cubes[i]->AddComponent(fallRb);
		world->AddRigidbody(cubes[i]->GetComponent<BulletRigidbody>()->GetBtRigidbody());
	}


	skull->AddComponent(new Model("Models/nanosuit/nanosuit.obj", modelShader, fpsCamera, skull->transform));
	Model* model = skull->GetComponent<Model>();

	skull->transform->SetPosition(glm::vec3(0.0f, -5.0f, 10.0f));


	GameObject* lamp = new GameObject();
	lamp->AddComponent(new Model("Models/lamp.obj", lampShader, fpsCamera, lamp->transform));

	Light sunLight = Light(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f),
		glm::vec3(1.0f, 0.67f, 1.0f),
		glm::vec3(0.25f)
	);

	sunLight.direction = glm::vec4(-0.2f, -1.0f, -0.3f, 1.0f);

	Light pointLight1 = Light(glm::vec3(0.0f, 20.0f, 20.0f), glm::vec3(10.0f), glm::vec3(100.0f, 0.0f, 0.0f), glm::vec3(0.1f));
	pointLight1.constant = 1.0f;
	pointLight1.linear = 0.07f;
	pointLight1.quadratic = 0.032f;

	Light pointLight2 = Light(glm::vec3(5.0f, 20.0f, 20.0f), glm::vec3(10.0f), glm::vec3(0.0f, 0.0f, 100.0f), glm::vec3(0.1f));
	pointLight2.constant = 1.0f;
	pointLight2.linear = 0.09f;
	pointLight2.quadratic = 0.032f;

	lamp->transform->SetPosition(pointLight1.position);
	std::vector<Light> lights;
	lights.push_back(pointLight1);
	lights.push_back(pointLight2);


	std::vector<char*> images;
	images.push_back("Textures/Right_MauveSpaceBox.png");
	images.push_back("Textures/Left_MauveSpaceBox.png");
	images.push_back("Textures/Up_MauveSpaceBox.png");
	images.push_back("Textures/Down_MauveSpaceBox.png");
	images.push_back("Textures/Back_MauveSpaceBox.png");
	images.push_back("Textures/Front_MauveSpaceBox.png");
	skybox = new Skybox(images);

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
			inputManager->prevKeyEvent = inputManager->currentKeyEvent;
			inputManager->currentKeyEvent = windowEvent.key;
			if (windowEvent.type == SDL_KEYDOWN)
			{
				if (windowEvent.key.keysym.sym == SDLK_ESCAPE)
					break;
			}


			if (windowEvent.type == SDL_MOUSEMOTION)
			{
				mouseX = windowEvent.motion.xrel;
				mouseY = windowEvent.motion.yrel;

				camRotation.x += mouseX * 4.0f * clock.GetDeltaTime();
				camRotation.y += mouseY * 4.0f * clock.GetDeltaTime();
			}

		}
		//

		// Game loop
		renderer.PreRender();
		skybox->Render(fpsCamera);
		renderer.Render();


		/////Camera Target
		if (focusOnMouse) {
			fpsCamera->Rotate(camRotation.x, camRotation.y, camRotation.z);
		}

		if (inputManager->IsKeyDown(SDLK_5)) {
			focusOnMouse = false;
			camRotation = fpsCamera->LookAt(lamp->transform->position); // camRotation needs to be done in camera class
																		// camRotation would have to be updated everytime you wanted to look at something 
																		// camRotation is set to the current yaw/pitch/roll of the camera or else
																		// it will snap back to where the previous mouse position was
																		// because it is calculated by the rel difference
		}
		else {
			focusOnMouse = true;
		}
		fpsCamera->Update();
		//////////////////////
		


		model->Render(sunLight, lights, skybox->textureID);
		skull->transform->Rotate(glm::vec3(0.0f, clock.GetDeltaTime(), 0.0f));

		lamp->GetComponent<Model>()->Render(sunLight, lights, skybox->textureID);

		for (int i = 0; i < NUM_OF_BOXES; i++)
		{
			cubes[i]->GetComponent<Model>()->Render(sunLight, lights, skybox->textureID);
		}

		//Bullet
		world->Step(clock.GetDeltaTime(), 10);
		//

		skull->Update((float)clock.GetDeltaTime());

		for (int i = 0; i < NUM_OF_BOXES; i++)
		{
			cubes[i]->Update((float)clock.GetDeltaTime());
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

		if (inputManager->IsKeyDown(SDLK_KP_8)) {

			lights[0].position.y += 10.0f * clock.GetDeltaTime();
			lights[1].position.y += 10.0f * clock.GetDeltaTime();
			lamp->transform->SetPosition(lights[0].position);
		}

		if (inputManager->IsKeyDown(SDLK_KP_2)) {

			lights[0].position.y -= 10.0f * clock.GetDeltaTime();
			lights[1].position.y -= 10.0f * clock.GetDeltaTime();
			lamp->transform->SetPosition(lights[0].position);
		}

		if (inputManager->IsKeyDown(SDLK_KP_4)) {

			lights[0].position.x += 10.0f * clock.GetDeltaTime();
			lights[1].position.x += 10.0f * clock.GetDeltaTime();
			lamp->transform->SetPosition(lights[0].position);
		}

		if (inputManager->IsKeyDown(SDLK_KP_6)) {

			lights[0].position.x -= 10.0f * clock.GetDeltaTime();
			lights[1].position.x -= 10.0f * clock.GetDeltaTime();
			lamp->transform->SetPosition(lights[0].position);
		}

		if (inputManager->IsKeyDown(SDLK_KP_1)) {

			lights[0].position.z += 10.0f * clock.GetDeltaTime();
			lights[1].position.z += 10.0f * clock.GetDeltaTime();
			lamp->transform->SetPosition(lights[0].position);
		}

		if (inputManager->IsKeyDown(SDLK_KP_3)) {

			lights[0].position.z -= 10.0f * clock.GetDeltaTime();
			lights[1].position.z -= 10.0f * clock.GetDeltaTime();
			lamp->transform->SetPosition(lights[0].position);
		}

		if (inputManager->IsKeyDown(SDLK_z))
		{
			lights[0].diffuse = glm::vec3(0.0f, 100.0f, 0.0f);
		}
		if (inputManager->IsKeyDown(SDLK_x))
		{
			lights[0].diffuse = glm::vec3(100.0f, 0.0f, 0.0f);
		}
		if (inputManager->IsKeyDown(SDLK_SPACE))
		{			
			for (int i = 0; i < NUM_OF_BOXES; i++)
			{
				cubes[i]->GetComponent<BulletRigidbody>()->ApplyTorque(btVector3(0, i * 15, 0));
			}
		}



		//Follow camera for testing purposes
		/*glm::vec3 cubePos = cubes[0]->transform->position;
		cubePos.y += 10;
		cubePos.z -= 10;
		fpsCamera->SetPosition(cubePos);*/



		renderer.PostRender();
		//

		window->Refresh();

	}

	for (int i = 0; i < NUM_OF_BOXES; i++)
	{
		delete cubes[i];
	}
	delete skybox;
	delete skull;
	delete model;
	delete fpsCamera;
	quitGame();

	return 0;
}