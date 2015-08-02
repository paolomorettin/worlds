#pragma once

#include <bullet/LinearMath/btDefaultMotionState.h>
#include <string>
#include <set>
#include <irrlicht.h>
#include "EventReceiver.hpp"

class GameLoop;
class btCollisionObject;

class IGameObject : public btMotionState{
    std::set<std::pair<EventReceiver*, irr::EEVENT_TYPE>> observed_events;

    void observe(EventReceiver *evt_recv, irr::EEVENT_TYPE evt_t) {
        std::pair<EventReceiver*, irr::EEVENT_TYPE> evt_pair;
        evt_pair = std::make_pair(evt_recv, evt_t);
        if (observed_events.find(evt_pair) == observed_events.end()) {
            evt_recv->attach(this, evt_t);
            observed_events.insert(evt_pair);
        }
    }

    void deobserve(EventReceiver *evt_recv, irr::EEVENT_TYPE evt_t) {
        std::pair<EventReceiver*, irr::EEVENT_TYPE> evt_pair;
        evt_pair = std::make_pair(evt_recv, evt_t);
        if (observed_events.find(evt_pair) == observed_events.end()) {
            evt_recv->detach(this, evt_t);
            observed_events.erase(evt_pair);
        }
    }

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


    virtual void notify(const irr::SEvent& evt) = 0;

    //! Called with a fixed rate of 300Hz
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


    // Called to notify that a collision happened.
    virtual void collision_callback(const btCollisionObject* other_obj) {
        // do nothing by default.
    }

    virtual ~IGameObject() {
        for (auto evt : observed_events) {
            evt.first->detach(this, evt.second);
        }
    };
};

