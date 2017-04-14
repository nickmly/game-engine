#include "PhysicsWorld.h"
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>


void PhysicsWorld::Step(float timeStep, int maxSubSteps)
{
	dynamicsWorld->stepSimulation(timeStep, maxSubSteps);
}

void PhysicsWorld::AddRigidbody(btRigidBody* rb)
{
	dynamicsWorld->addRigidBody(rb);
}

PhysicsWorld::PhysicsWorld()
{
	broadphase = new btDbvtBroadphase();
	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);

	solver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));
}


PhysicsWorld::~PhysicsWorld()
{
	delete dynamicsWorld;
	delete solver;
	delete dispatcher;
	delete collisionConfig;
	delete broadphase;
}
