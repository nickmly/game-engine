#pragma once

class Component
{
public:
	char* name = "Component";
	virtual void Update(float deltaTime);
};