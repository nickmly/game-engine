#include "BulletRigidbody.h"



void BulletRigidbody::Init(const btVector3 &position, const btQuaternion &rotation)
{
	defaultMotionState = new btDefaultMotionState(btTransform(rotation, position));
	

	btVector3 localInertia = ConvertVector(inertia);
	shape->calculateLocalInertia(mass, localInertia);
	btRigidBody::btRigidBodyConstructionInfo rigidbodyCI(mass, defaultMotionState, shape, localInertia);
	rigidbody = new btRigidBody(rigidbodyCI);
}

void BulletRigidbody::Update(float deltaTime)
{
}

BulletRigidbody::BulletRigidbody(btCollisionShape* _shape, const btVector3 &position, const btQuaternion &rotation, float _mass)
{
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

btVector3 BulletRigidbody::ConvertVector(glm::vec3 _vec)
{
	return btVector3(_vec.x, _vec.y, _vec.z);
}
