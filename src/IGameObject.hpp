#pragma once

#include <bullet/LinearMath/btDefaultMotionState.h>
#include <string>

class GameLoop;

class IGameObject : public btMotionState{

 public:
    btTransform m_graphicsWorldTrans;
    btTransform m_centerOfMassOffset;
    btTransform m_startWorldTrans;

    std::string name;

    explicit IGameObject(std::string name = std::string("[unknown]"))
        : btMotionState(), name(name),
          m_graphicsWorldTrans(btTransform::getIdentity()),
          m_centerOfMassOffset(btTransform::getIdentity()),
          m_startWorldTrans(btTransform::getIdentity())
    {
    }

    //! Called with a fixed rate of 100Hz
    /*!
     * Here you should handle the logic, the network and all the
     * non-graphic-related tasks.
     */
    virtual void logic_tick(GameLoop& mgr) = 0;

    ///synchronizes world transform from user to physics
    // Called by bullet
    virtual void getWorldTransform(btTransform& centerOfMassWorldTrans ) const
    {
            centerOfMassWorldTrans = m_centerOfMassOffset.inverse() * m_graphicsWorldTrans ;
    }

    ///synchronizes world transform from physics to user
    // called by bullet
    ///Bullet only calls the update of worldtransform for active objects
    virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
    {
            m_graphicsWorldTrans = centerOfMassWorldTrans * m_centerOfMassOffset ;
    }


    virtual ~IGameObject() {};
};

