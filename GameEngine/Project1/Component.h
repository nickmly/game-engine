#pragma once

class GameObject;
class Component
{
public:
	GameObject* parent;
	char* name = "Component";
	virtual void Update(float deltaTime);
};