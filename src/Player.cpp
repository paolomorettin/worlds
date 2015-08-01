#include "Player.hpp"
#include "LevelGenerator.hpp"
#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
#include <iostream>

void PlayerGameObj::logic_tick(GameLoop& loop) {

    // for testing purposes
    float movement_velocity = 1.0;
    
    irr::core::vector3df cd = camera -> getTarget();
    btVector3 direction = btVector3(1000,0,0);

    core::vector3df camPosition = camera->getPosition();
    
    // get the 'position' of the thing that the camera (node) is pointed at
    irr::core::vector3df camTarget = camera->getTarget();

    // figure out the 'direction' vector that describes the relative position of the camera to it's target:
    irr::core::vector3df camDirection = camTarget - camPosition;

    // scale the direction vector for frame-rate independent speed:
    irr::core::vector3df camMovement = camDirection.normalize();
    camMovement = camMovement * movement_velocity;

    btVector3 movement = btVector3(camMovement.X, camMovement.Y, camMovement.Z);


    std::cout <<"Endl:" << movement.x() <<" " << movement.y() <<" " << movement.z() << std::endl;
    // check for keyboard input
    if(loop.evrecv -> IsKeyDown(irr::KEY_KEY_W)){
        rigid_body -> applyCentralImpulse(movement);
        printf("W\n");
    } else if(loop.evrecv -> IsKeyDown(irr::KEY_KEY_S))
        printf("S\n");

    if(loop.evrecv -> IsKeyDown(irr::KEY_KEY_A))
        printf("A\n");
    else if(loop.evrecv -> IsKeyDown(irr::KEY_KEY_D))
        printf("D\n");
}

void PlayerGameObj::render(GameLoop&, float) {
    // all done by irr
}

btRigidBody* PlayerGameObj::initialize(GameLoop& loop, const vector3df& start_pos) {

    SKeyMap keyMap[10];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;

    keyMap[8].Action = EKA_CROUCH;
    keyMap[8].KeyCode = KEY_SHIFT;

    keyMap[9].Action = EKA_JUMP_UP;
    keyMap[9].KeyCode = KEY_SPACE;

    int ID_camera = 0; // <- are you kidding me? no.
    this->camera = loop.smgr -> addCameraSceneNodeFPS(0, 100, 0.3, ID_camera, keyMap, 0, true, 10);
    this->camera->setFarValue(200);
    this->camera->setNearValue(0.01);
    int ID_playerlight = 0;
    this->playerlight = loop.smgr->addLightSceneNode(camera, core::vector3df(0,0,0), video::SColor(255,255,255,255), 3.0f, ID_playerlight);



	
	loop.attach(this);
	

	// Only a rename, sorry!
	const vector3df& sp = start_pos;
    // player obj test
    // TODO: All of the following should be moved into playergameobj.initialize 
    //std::cout<<"START PLAYER POS:"<<sp.X<<","<<sp.Y<<","<<sp.Z<<","<<std::endl;
    // from euler angles, other constructors should be preferred (I
    // think)
    btQuaternion init_rotation(btScalar(0.01),btScalar(0.01),btScalar(0.01));
    // initial position of the rigid body.
    btVector3 init_position(btScalar(sp.X), btScalar(sp.Y), btScalar(sp.Z));
    // initialize the rigid body transform.
    btTransform transform(init_rotation, init_position);
    // set motion state.
    setWorldTransform(transform);

    // model of the player in the physicial world: A BIG ROUND SPHERE: you fat!
    btCollisionShape* sphere = new btSphereShape(0.2);

    // inertia vector.
    btVector3 inertiavector(0.1,0.1,0.1);

    // add the rigid body
    // mass of 80 kg.
    rigid_body = new btRigidBody(80, this, sphere, inertiavector);
	return rigid_body;

}
