
#pragma once



#include <irrlicht.h>
#include <vector>
#include <memory>
#include <unordered_map>

#include "EventReceiver.hpp"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class IGameObject;
class btRigidBody;
class btCollisionObject;

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
    std::unordered_map<const btCollisionObject*, IGameObject*> collision_objs; // those who receive collision callbacks

    void attach(IGameObject*);
    void detach(IGameObject*);


    void register_collision_callback(btCollisionObject*, IGameObject*);
    void remove_collision_callback(btCollisionObject*, IGameObject*);
    void handle_collisions();
    
    bool initialize_irrlicht(/* config manager */);
    bool initialize_bullet(/* config manager */);
    void start_loop();

    inline EventReceiver* get_event_receiver() {
        return ev_recv;
    }

};


