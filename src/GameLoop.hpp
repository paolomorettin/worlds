
#pragma once



#include <irrlicht.h>
#include <vector>
#include <memory>

#include "EventReceiver.hpp"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class IGameObject;

using namespace irr;

class GameLoop {
    EventReceiver *ev_recv;
    public:

    GameLoop();
    ~GameLoop();

    IrrlichtDevice *device;
    video::IVideoDriver* driver;
    gui::IGUIEnvironment* guienv;
    scene::ISceneManager* smgr;
    ITimer* timer;

    btDefaultCollisionConfiguration* config;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingpaircache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    std::vector<IGameObject*> event_objs;

    void attach(IGameObject*);
    void detach(IGameObject*);

    bool initialize_irrlicht(/* config manager */);
    bool initialize_bullet(/* config manager */);
    void start_loop();

    inline EventReceiver* get_event_receiver() {
        return ev_recv;
    }

};


