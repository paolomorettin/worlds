#pragma once

#include "GameLoop.hpp"
#include "IGameObject.hpp"
#include "LevelGenerator.hpp"

class LevelTimer;
class btRigidBody;

class Player : public IGameObject {
    scene::ICameraSceneNode *camera; // irr camera
    scene::ILightSceneNode *playerlight; // irr light that follows the player.
    btRigidBody *rigid_body;
    LevelTimer* level_timer;
    typedef enum {
        FORWARD = 0,
        BACKWARD,
        RIGHT,
        LEFT,
        JUMP,
        NUM_OF_MOVES
    } movements;
    bool move_cmd[NUM_OF_MOVES];

    void handle_key_event(const SEvent&);

 public:

    explicit Player():
        IGameObject("Player"), playerlight(nullptr), camera(nullptr), rigid_body(nullptr)
    {
        for (int i=0; i<NUM_OF_MOVES; i++) {
            move_cmd[i] = false;
        }
    }

    // virtual (override of IGameObject)
    // called by bullet
    virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
    {
        IGameObject::setWorldTransform(centerOfMassWorldTrans);
        btVector3 pos = m_graphicsWorldTrans.getOrigin();
        camera->setPosition(core::vector3df(pos.x(), pos.y(), pos.z()));
    }

    virtual void render(GameLoop&, float);
    virtual btRigidBody* initialize(GameLoop&, const vector3df& start_pos, LevelTimer* timer);
    virtual void notify(const SEvent&);
    virtual void logic_tick(GameLoop&);

    virtual void collision_callback(const btCollisionObject* obj);

    virtual ~Player() {};
};

