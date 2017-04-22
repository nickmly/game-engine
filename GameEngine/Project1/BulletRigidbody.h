#pragma once
#include <btBulletDynamicsCommon.h>
#include <glm.hpp>
#include "Component.h"

class BulletRigidbody : public Component
{
private:
	btDefaultMotionState* defaultMotionState;
	btRigidBody* rigidbody;
	btCollisionShape* shape;
	void Init(const btVector3 &position, const btQuaternion &rotation);
public:
	float mass = 0.0f;
	glm::vec3 inertia = glm::vec3(0.0f);

	virtual void Update(float deltaTime) override;
	BulletRigidbody(btCollisionShape* _shape, const btVector3 &position, const btQuaternion &rotation, float _mass = 1.0f);
	~BulletRigidbody();

	btRigidBody* GetBtRigidbody();
	void GetTransform(btTransform &trans);
	btVector3 GetPosition();
	btVector3 GetCenterOfMass();

	void ApplyTorque(const btVector3 &torque);
	void ApplyTorqueImpulse(const btVector3 &torque);
	void ApplyForce(const btVector3 &force);
	void ApplyForce(const btVector3 &force, const btVector3 &relativePos);
	void ApplyImpulse(const btVector3 &impulse);
	void ApplyImpulse(const btVector3 &impulse, const btVector3 &relativePos);


	btVector3 ConvertVectorToBT(glm::vec3 _vec);
	glm::vec3 ConvertVectorToGLM(const btVector3 &_vec);
};

