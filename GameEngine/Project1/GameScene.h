#pragma once

class GameScene
{
public:
	virtual void OnCreate() = 0;
	virtual void OnStart() = 0;
	virtual void OnUpdate(float delta) = 0;
	virtual void OnPreRender() = 0;
	virtual void OnRender() = 0;
	virtual void OnPostRender() = 0;
	virtual void OnQuit() = 0;
	GameScene();
	~GameScene();
};

