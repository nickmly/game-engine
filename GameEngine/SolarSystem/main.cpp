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


const char* TITLE = "Window Title";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int NUM_OF_BOXES = 8;

TTF_Font *font;

Window* window;

int main(int argc, char** argv)
{
	return 0;
}