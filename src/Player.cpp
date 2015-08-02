#include "Player.hpp"
#include "LevelGenerator.hpp"
#include "LevelEnd.hpp"
#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
#include <iostream>
#include <irrlicht.h>
#include <iostream>
#include <cmath>


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
    float movement_velocity = 0.1;
    float jump_strength = 100.00;

    irr::core::vector3df cd = camera -> getTarget();

    core::vector3df camPosition = camera->getPosition();

    // get the 'position' of the thing that the camera (node) is pointed at
    irr::core::vector3df camTarget = camera->getTarget();

    // figure out the 'direction' vector that describes the relative position of the camera to it's target:
    irr::core::vector3df camDirection = camTarget - camPosition;

    // Don't allow the modification of up/down velocity. Flatten down the camera vector to the XZ plane.
    camDirection.Y = 0;

    // scale the direction vector to a unit vector & multiply velocity.
    irr::core::vector3df camMovement = camDirection.normalize();
    camMovement = camMovement;

    // Convert to a bullet vector
    btVector3 direction = btVector3(camMovement.X, camMovement.Y, camMovement.Z);
    btVector3 movement = btVector3(0,0,0);

    // check for keyboard input
    if (move_cmd[FORWARD]) {
        movement += direction;
    } else if (move_cmd[BACKWARD]) {
        movement -= direction;
    }

    if (move_cmd[RIGHT]) {
        movement += direction.rotate(btVector3(0,1,0), 90);
    } else if (move_cmd[LEFT]) {
        movement -= direction.rotate(btVector3(0,1,0), 90);
    }

    if (movement != btVector3(0,0,0)) {
        movement = movement.normalize()* movement_velocity;
    }

    // Get the previous velocity. The acceleration we get depends on this.
    const btVector3& velocity = rigid_body->getLinearVelocity();

    const btVector3 air_resistance((velocity.x()*velocity.x()*velocity.x())/100,
                                   (velocity.y()*velocity.y()*velocity.y())/100,
                                   (velocity.z()*velocity.z()*velocity.z())/100);
    movement -= air_resistance;
    if (movement != btVector3(0,0,0)) {
        // Speed modulation: get the "coefficient of change"
        // 1 = trying to accelerate in the reverse direction of the movement.
        // 0.5 = perpendicolar (or was stopped)
        // 0 = in the same direction
        
        rigid_body -> applyCentralImpulse(movement);
    }

    if (move_cmd[JUMP] && velocity.y() < 0 ) {
        rigid_body -> applyCentralImpulse(btVector3(0, jump_strength, 0));
    }

    if(camPosition.Y < -10) {
        std::cout<<"You lose(r)!"<<std::endl;
        exit(0);
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
    btCollisionShape* sphere = new btSphereShape(0.4);

    // inertia vector:
    // mass of 100 kg.
    const btScalar mass = 100;
    btVector3 inertiavector(0,0,0);
    sphere->calculateLocalInertia(mass, inertiavector);

    // add the rigid body
    // mass of 80 kg.
    rigid_body = new btRigidBody(mass, this, sphere, inertiavector);
    rigid_body->setActivationState(DISABLE_DEACTIVATION);

    loop.register_collision_callback(rigid_body, this);
    return rigid_body;

}


void PlayerGameObj::collision_callback(const btCollisionObject* obj) {
    const btRigidBody* rb = btRigidBody::upcast(obj);
    const btMotionState* motion = nullptr;
    const IGameObject* gobj = nullptr;
    if(rb) {
        motion = rb->getMotionState();
        gobj = dynamic_cast<const IGameObject*> (motion);
    }
    
    if(gobj) {
        // do something other than the end?
        const LevelEndObj* end = dynamic_cast<const LevelEndObj*>(gobj);
        if(end) {
            std::cout<<"You win!"<<std::endl;
        }
    }
}
