/*
Bullet Continuous Collision Detection and Physics Library
Copyright (c) 2003-2007 Erwin Coumans  http://continuousphysics.com/Bullet/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#include "btBulletDynamicsCommon.h"
#include <stdio.h>

btDiscreteDynamicsWorld* dynamicsWorld; 

void initializeBulletWorld() { 


	int i;

	///collision configuration contains default setup for memory, collision setup. Advanced users can create their own configuration.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();

	///use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);

	///btDbvtBroadphase is a good general purpose broadphase. You can also try out btAxis3Sweep.
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();

	///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0,-10,0));

	///create a few basic rigid bodies
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1000),btScalar(10),btScalar(1000)));
  //btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!
	btAlignedObjectArray<btCollisionShape*> collisionShapes;

	collisionShapes.push_back(groundShape);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,-10,0));

	{
		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass,localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);


    rbInfo.m_restitution = 0.5;
    rbInfo.m_friction = 0.5;

		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	///create a few basic rigid bodies
	groundShape = new btBoxShape(btVector3(btScalar(50),btScalar(50),btScalar(50)));
  //btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!

	collisionShapes.push_back(groundShape);

	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(80,0,0));

	{
		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass,localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);


    rbInfo.m_restitution = 0.5;
    rbInfo.m_friction = 0.5;

		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	groundShape = new btBoxShape(btVector3(btScalar(50),btScalar(50),btScalar(50)));
  //btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!

	collisionShapes.push_back(groundShape);

	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(-80,0,0));

	{
		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass,localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);


    rbInfo.m_restitution = 0.5;
    rbInfo.m_friction = 0.5;

		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}


	groundShape = new btBoxShape(btVector3(btScalar(50),btScalar(50),btScalar(50)));
  //btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);

	//keep track of the shapes, we release memory at exit.
	//make sure to re-use collision shapes among rigid bodies whenever possible!

	collisionShapes.push_back(groundShape);

	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,100,0));

	{
		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass,localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,groundShape,localInertia);


    rbInfo.m_restitution = 0.5;
    rbInfo.m_friction = 0.5;

		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}



	{
		//create a dynamic rigidbody

		//btCollisionShape* colShape = new btBoxShape(btVector3(1,1,1));
    int num_obj = 60;
    for (int i = 0; i < num_obj; i++) { 

      btCollisionShape* colShape = new btSphereShape(btScalar(2.));
      collisionShapes.push_back(colShape);

      /// Create Dynamic Objects
      btTransform startTransform;
      startTransform.setIdentity();

      btScalar	mass(1.f);

      //rigidbody is dynamic if and only if mass is non zero, otherwise static
      bool isDynamic = (mass != 0.f);

      btVector3 localInertia(0,0,0);
      if (isDynamic)
        colShape->calculateLocalInertia(mass,localInertia);

      startTransform.setOrigin(btVector3(-17.5 + (i % 10) * 4,30 + 4*(i / 10),0));

      //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
      btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
      btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
      //btRigidBody::btRigidBodyConstructionInfo rbInfo;

      rbInfo.m_restitution = 1;
      rbInfo.m_friction = 1.5;
      rbInfo.m_mass = 0.5;



      btRigidBody* body = new btRigidBody(rbInfo);
      if (i % 2 == 0) { 
      
        btVector3 initVel(10*drand48(), -10*drand48(), 0);

        body->setLinearVelocity(initVel);
      } else { 

        btVector3 initVel(-10*drand48(), -10*drand48(), 0);

        body->setLinearVelocity(initVel);
      }

      dynamicsWorld->addRigidBody(body);
    }
  }

}

void step_simulation(vector<vector<double> >& sphere_offsets) {
/// Do some simulation

		dynamicsWorld->stepSimulation(1.f/60.f,10);
		
		//print positions of all objects
		for (int j=dynamicsWorld->getNumCollisionObjects()-1; j>=0 ;j--)
		{
      if ( j == 0) continue;

      vector<double> offsets;
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			if (body && body->getMotionState())
			{
				btTransform trans;
				body->getMotionState()->getWorldTransform(trans);
        offsets.push_back(trans.getOrigin().getX());
        offsets.push_back(trans.getOrigin().getY());
      }
      sphere_offsets.push_back(offsets);
		}

}

