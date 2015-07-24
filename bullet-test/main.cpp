#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
#include <iostream>


#define PRINTVEC(vec) vec.x()<<","<<vec.y()<<","<<vec.z()
int main() {
	btDefaultCollisionConfiguration* config = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(config);
	btBroadphaseInterface* overlappingpaircache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingpaircache, solver, config);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	/////////////////////// RB1 ////////////////
	{
		// from euler angles, other constructors should be preferred (I
		// think)
		btQuaternion init_rotation(btScalar(0),btScalar(0),btScalar(0));
		// initial position of the rigid body.
		btVector3 init_position(btScalar(0), btScalar(0), btScalar(0));
		// initialize the rigid body transform.
		btTransform transform(init_rotation, init_position);
		// motion state. Holds the current rotation and position of the
		// object. I think they created this class to allow overriding the
		// setWorldTransform to sync with graphics.
		btDefaultMotionState* motion_state = new btDefaultMotionState(transform);
		motion_state->m_userPointer = (void*)"Sphere";
		// Shape
		btCollisionShape* sphere = new btSphereShape(0.1);

		// inertia vector.
		btVector3 inertiavector(0.01,0.1,0.1);
		
		// add the rigid body. mass is 1.
		btRigidBody* test = new btRigidBody(1, motion_state, sphere, inertiavector);
		dynamicsWorld->addRigidBody(test);

		// just as a test, start with some initial velocity
		test->applyCentralImpulse(btVector3(1, 5, 1));
	}

	/////////////////////// RB2: a fixed position box ////////////////
	{
		// from euler angles, other constructors should be preferred (I
		// think)
		btQuaternion init_rotation(btScalar(0),btScalar(0),btScalar(0));
		// initial position of the rigid body.
		btVector3 init_position(btScalar(0), btScalar(-2), btScalar(0));
		// initialize the rigid body transform.
		btTransform transform(init_rotation, init_position);
		// motion state. Holds the current rotation and position of the
		// object. I think they created this class to allow overriding the
		// setWorldTransform to sync with graphics.
		btDefaultMotionState* motion_state = new btDefaultMotionState(transform);
		motion_state->m_userPointer = (void*)"Box";
		// Shape
		btCollisionShape* sphere = new btBoxShape(btVector3(10,1,10));

		// add the rigid body
		// mass of 0 means that the body is a fixed body.
		btRigidBody* test = new btRigidBody(0, motion_state, sphere);
		dynamicsWorld->addRigidBody(test);
	}

	for(int i = 0; i<100; i++) {
		std::cout<<"step "<<i<<":"<<std::endl;
		dynamicsWorld->stepSimulation(1.f/60.f, 10);
		btAlignedObjectArray<btCollisionObject*> objs = dynamicsWorld->getCollisionObjectArray();
		for (int j = 0; j < dynamicsWorld->getNumCollisionObjects(); j++) {
			btRigidBody* body = ((btRigidBody*)objs[j]);
			const btVector3& position = body->getCenterOfMassPosition();
			const btVector3& linearvelocity = body->getLinearVelocity();
			const btVector3& angualrvelocity = body->getAngularVelocity();
			btMotionState* mstate = body->getMotionState();
			btDefaultMotionState* defmstate;
			char* itemname = nullptr;
			if ( (defmstate = dynamic_cast<btDefaultMotionState*>(mstate)) ) {
				itemname = (char*) defmstate->m_userPointer;
			}
			if (itemname == nullptr) {
				itemname = (char*) "[Unknown]";
			}
			std::cout<<" - "<<itemname<<std::endl;
			std::cout<<"    - pos   : "<<PRINTVEC(position) << std::endl;
			std::cout<<"    - linvel: "<<PRINTVEC(linearvelocity) << std::endl;
			std::cout<<"    - angvel: "<<PRINTVEC(angualrvelocity) << std::endl;
		}

	}
	return 0;
}
