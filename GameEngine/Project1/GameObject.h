#pragma once
#include "Component.h"

#include <vector>
#include <memory>
#include <iostream>

class GameObject
{
private:
	std::vector<Component*> components;
public:
	void AddComponent(Component* c)
	{
		components.push_back(c);
	};

	template<typename T>
	T* GetComponent()
	{
		for (int i = 0; i < components.size(); i++)
		{
			T* c = dynamic_cast<T*>(components.at(i));
			if (c != nullptr)
				return c;
		}

	};

	void Update(float deltaTime)
	{
		for (int i = 0; i < components.size(); i++)
		{
			//std::cout << components.at(i)->name << " updating" << std::endl;
			components.at(i)->Update(deltaTime);
		}
	};
	GameObject() {};
	~GameObject() {};
};


