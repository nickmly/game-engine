#pragma once
class Collider
{
public:
	Collider();
	~Collider();
	virtual bool IsColliding(Collider otherCollider);
};

