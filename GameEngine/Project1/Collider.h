#pragma once
#include "Component.h"

class Collider : public Component
{
public:
	Collider();
	~Collider();
	virtual bool IsColliding(Collider otherCollider);
	virtual void Update(float deltaTime) override;
};

