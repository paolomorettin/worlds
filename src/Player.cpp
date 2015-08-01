#include "Player.hpp"
#include "LevelGenerator.hpp"
#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
#include <iostream>
#include <irrlicht.h>


void PlayerGameObj::notify (const SEvent& evt) {
    if (evt.EventType == irr::EET_KEY_INPUT_EVENT)
        handle_key_event(evt);
    else
        printf("WARNING: unexpected event %d not handled by player\n", evt.EventType);
}

void PlayerGameObj::handle_key_event(const SEvent& evt) {
    unsigned cmd;
    switch (evt.KeyInput.Key) {
        case irr::KEY_KEY_W:
            cmd = FORWARD;
            break;
        case irr::KEY_KEY_S:
            cmd = BACKWARD;
            break;
        case irr::KEY_KEY_A:
            cmd = LEFT;
            break;
        case irr::KEY_KEY_D:
            cmd = RIGHT;
            break;
        case irr::KEY_SPACE:
            cmd = JUMP;
            break;
        default:
            return;
    }
    move_cmd[cmd] = evt.KeyInput.PressedDown;
}

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
    if (move_cmd[FORWARD]) {
        rigid_body -> setLinearVelocity(movement);
    } else if (move_cmd[BACKWARD]) {
        rigid_body -> setLinearVelocity(-movement);
    }

    if (move_cmd[RIGHT]) {
        printf("A\n");
    } else if (move_cmd[LEFT]) {
        printf("D\n");
    }

    if (move_cmd[JUMP]) {
        printf("Jump like you were in Van Halen!\n");
    }
}

void PlayerGameObj::render(GameLoop&, float) {
    // all done by irr
}

btRigidBody* PlayerGameObj::initialize(GameLoop& loop, const vector3df& start_pos) {
    (loop.get_event_receiver())->attach(this, irr::EET_KEY_INPUT_EVENT);

    SKeyMap *keyMap;
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
