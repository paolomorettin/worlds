#pragma once

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

#include <irrlicht.h>
#include <vector>

class IGameObject;

using namespace irr;

#define NUM_OF_IRR_EVENTS 6

class EventReceiver : public IEventReceiver {
    std::vector<IGameObject*> observers[NUM_OF_IRR_EVENTS];

 public:
    virtual bool OnEvent(const SEvent& event);
    void attach(IGameObject* obj, irr::EEVENT_TYPE evt_t);
    void detach(IGameObject* obj, irr::EEVENT_TYPE evt_t);
};
