#pragma once
#include <btBulletDynamicsCommon.h>

class PhysicsWorld
{
private:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfig;
	btCollisionDispatcher* dispatcher;

	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
public:
	void Step(float timeStep, int maxSubSteps = 1);
	void AddRigidbody(btRigidBody* rb);
	PhysicsWorld();
	~PhysicsWorld();
};

