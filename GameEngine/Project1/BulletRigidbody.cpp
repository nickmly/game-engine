#include "BulletRigidbody.h"
#include "GameObject.h"


void BulletRigidbody::Init(const btVector3 &position, const btQuaternion &rotation)
{
	defaultMotionState = new btDefaultMotionState(btTransform(rotation, position));
	

	btVector3 localInertia = ConvertVectorToBT(inertia);
	shape->calculateLocalInertia(mass, localInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidbodyCI(mass, defaultMotionState, shape, localInertia);
	rigidbody = new btRigidBody(rigidbodyCI);
}

void BulletRigidbody::Update(float deltaTime)
{
	btTransform trans;
	GetTransform(trans);

	parent->transform->SetPosition(ConvertVectorToGLM(trans.getOrigin()));
	
	parent->transform->SetRotation(glm::vec3(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ()));
}

BulletRigidbody::BulletRigidbody(btCollisionShape* _shape, const btVector3 &position, const btQuaternion &rotation, float _mass)
{
	name = "BulletRigidbody";
	mass = _mass;
	shape = _shape;
	Init(position, rotation);
}

BulletRigidbody::~BulletRigidbody()
{
}

btRigidBody* BulletRigidbody::GetBtRigidbody()
{
	return rigidbody;
}

void BulletRigidbody::GetTransform(btTransform & trans)
{
	rigidbody->getMotionState()->getWorldTransform(trans);
}

btVector3 BulletRigidbody::GetPosition()
{
	btTransform trans;
	rigidbody->getMotionState()->getWorldTransform(trans);
	return trans.getOrigin();
}

btVector3 BulletRigidbody::GetCenterOfMass()
{
	return rigidbody->getCenterOfMassPosition();
}

void BulletRigidbody::ApplyTorque(const btVector3 & torque)
{
	rigidbody->applyTorque(torque);
}

void BulletRigidbody::ApplyTorqueImpulse(const btVector3 & torque)
{
	rigidbody->applyTorqueImpulse(torque);
}

void BulletRigidbody::ApplyForce(const btVector3 & force)
{
	rigidbody->applyCentralForce(force);
}

void BulletRigidbody::ApplyForce(const btVector3 & force, const btVector3 & relativePos)
{
	rigidbody->applyForce(force, relativePos);
}

void BulletRigidbody::ApplyImpulse(const btVector3 & impulse)
{
	rigidbody->applyCentralImpulse(impulse);
}

void BulletRigidbody::ApplyImpulse(const btVector3 & impulse, const btVector3 & relativePos)
{
	rigidbody->applyImpulse(impulse, relativePos);
}


btVector3 BulletRigidbody::ConvertVectorToBT(glm::vec3 _vec)
{
	return btVector3(_vec.x, _vec.y, _vec.z);
}

glm::vec3 BulletRigidbody::ConvertVectorToGLM(const btVector3 &_vec)
{
	return glm::vec3(_vec.getX(), _vec.getY(), _vec.getZ());
}
