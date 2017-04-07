#include "Rigidbody.h"


Rigidbody::~Rigidbody()
{
}

void Rigidbody::Update(float deltaTime)
{
	acceleration = force / mass;

	initialVelocity = velocity + acceleration*(float)deltaTime;
	
	glm::vec3 newPos = initialVelocity * (float)deltaTime + (0.5f * acceleration)*(float)(deltaTime*deltaTime);
	//position += newPos;
	
	velocity = initialVelocity;

	//velocity *= 0.000000000009f;
	//printf("%f\n", velocity.x);
	//std::cout << "newPosition:  " << position.x << std::endl;
	transform->SetPosition(newPos);
	force = glm::vec3(0.0f);
}

void Rigidbody::AddForce(glm::vec3 _force)
{
	force = _force;
}


