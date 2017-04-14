#pragma once
#include <memory>
#include "OpenGLRenderer.h"
#include "AbstractScene.h"
#include "SDL.h"
#include <iostream>
#include "Clock.h"
#include <vector>

class SceneManager
{
	private:
		SceneManager();
		~SceneManager();

	public:
		SceneManager(const SceneManager&) = delete;
		SceneManager(SceneManager&&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;
		SceneManager& operator=(SceneManager&&) = delete;

	private:
		//creates points to addresses of main.cpp game loop variables
		OpenGLRenderer *renderer;
		AbstractScene *currentScene;
		SDL_Event *inputEvent;

		///std::unique_ptr is a smart pointer that destroys the object it point to when the unique_ptr goes out of scope.
		static std::unique_ptr<SceneManager> instance;

		/// Since my destructor is private the template std::unique_ptr needs access so I made it a friend.
		/// However, std::default_delete is the default destruction policy used by std::unique_ptr 
		/// when no deleter is specified, therefore I'll make std::default_delete my friend as well. 
		friend std::default_delete<SceneManager>;

	private:
		bool isRunning;
		unsigned int fps; /// Frames / second
		std::vector<AbstractScene*> SceneList;
		Clock *GameClock;

	public:
		static SceneManager* getInstance();
		
		void InitializeScene(int sceneNumber);
		void StartScene();

		void Update();

		void EndScene();
		void StartNextScene();

		void AddScene(AbstractScene *scene);
		
		bool sceneChange = false;
		
		unsigned int SceneNumber;
		void SetScene(unsigned int sceneNumber);

		AbstractScene* GetCurrentScene();
};