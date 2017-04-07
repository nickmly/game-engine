#pragma once
#include "Component.h"
#include "BoxCollider.h"
#include "Transform.h"

#include <glm.hpp>
class Rigidbody : public Component
{
public:
	float mass = 100.0f;
	Transform* transform;
	
	glm::vec3 position, rotation, force;
	glm::vec3 initialVelocity, velocity, acceleration;

	Rigidbody() : position(0.0f), rotation(0.0f), velocity(0.0f), acceleration(0.0f), mass(1.0f) { name = "Rigidbody"; };
	Rigidbody(glm::vec3 _pos, glm::vec3 _rot) : position(_pos), rotation(_rot), velocity(0.0f), acceleration(0.0f), mass(1.0f) { name = "Rigidbody"; };
	
	Rigidbody(Transform* _transform) : transform(_transform), velocity(0.0f), acceleration(0.0f), mass(1.0f) {
		position = transform->position;
		initialVelocity = glm::vec3(0.0f);
		name = "Rigidbody"; 
	};
	
	~Rigidbody();
	virtual void Update(float deltaTime) override;
	void AddForce(glm::vec3 _force);

};

