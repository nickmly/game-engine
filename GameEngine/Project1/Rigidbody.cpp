#include "Rigidbody.h"


Rigidbody::~Rigidbody()
{
}

void Rigidbody::Update(float deltaTime)
{
	initialVelocity = velocity;

	acceleration = force / mass;
	velocity = initialVelocity + acceleration*(float)deltaTime;
	glm::vec3 newPos = velocity * (float)deltaTime + (0.5f * acceleration)*(float)(deltaTime*deltaTime);
	position += newPos;
	

	//printf("%f\n", velocity.x);
	transform->SetPosition(position);
}

void Rigidbody::AddForce(glm::vec3 _force)
{
	force = _force;
}


