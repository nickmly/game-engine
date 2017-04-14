#include "SceneManager.h"

/// See the header file reguarding unique_ptr
std::unique_ptr<SceneManager> SceneManager::instance(nullptr);

SceneManager* SceneManager::getInstance() {
	if (instance.get() == nullptr) {
		/// I originally set the unique_ptr to be null in the constructor - 
		/// reset() sets the new address
		instance.reset(new SceneManager());
	}
	return instance.get();
}

void SceneManager::InitializeScene(int sceneNumber)
{
	if(!GameClock)
	GameClock = new Clock();
	
	currentScene = SceneList.at(sceneNumber);
	currentScene->OnCreate();
}

void SceneManager::StartScene() {
		//Starts First scene in SceneList
		currentScene->OnStart();
}

void SceneManager::Update() {
	//renderScene
	currentScene->OnRender();
	//updateScene
	currentScene->OnUpdate(GameClock->GetDeltaTime());
}

void SceneManager::EndScene()
{
}

void SceneManager::StartNextScene()
{
}

void SceneManager::AddScene(AbstractScene *scene)
{
	SceneList.push_back(scene);
}

void SceneManager::SetScene(unsigned int sceneNumber) {
	//changes SceneNumber to desired scene number
	SceneNumber = sceneNumber;
	//sets current scene to desired scene number
	currentScene = SceneList.at(SceneNumber);
	if (!currentScene->HasStarted()) {
		//checks IsStarted in Base Scene.h onStart() only if the scene has not yet been started
		currentScene->OnStart();
	}
}

AbstractScene* SceneManager::GetCurrentScene()
{
	return currentScene;
}

SceneManager::~SceneManager() {
	renderer->Destroy();
}

SceneManager::SceneManager() {

}