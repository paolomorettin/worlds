#pragma once

#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include "IGameObject.hpp"
#include <irrlicht.h>

#include <iostream>

class LevelEndObj: public IGameObject {
    irr::scene::IMeshSceneNode* map_node;
    btRigidBody *rigid_body;
    public:

    explicit LevelEndObj():
        IGameObject("End"), map_node(nullptr)
    {
    }

    // virtual (override of IGameObject)
    // called by bullet
    virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
    {
        IGameObject::setWorldTransform(centerOfMassWorldTrans);
        btVector3 pos = m_graphicsWorldTrans.getOrigin();
        map_node->setPosition(core::vector3df(pos.x(), pos.y(), pos.z()));
    }


    virtual btRigidBody* initialize(GameLoop&, const irr::core::vector3df& position);
    virtual void notify(const irr::SEvent& evt);

    virtual ~LevelEndObj() {};
};

