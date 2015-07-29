
#pragma once



#include <irrlicht.h>
#include <vector>
#include <memory>

#include "GameObject.hpp"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

using namespace irr;

class GameLoop {
 public:

    IrrlichtDevice *device;
    video::IVideoDriver* driver;
    gui::IGUIEnvironment* guienv;
    scene::ISceneManager* smgr;
    ITimer* timer;
	IEventReceiver* evrecv;

	btDefaultCollisionConfiguration* config;
	btCollisionDispatcher* dispatcher;
	btBroadphaseInterface* overlappingpaircache;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;
    
	bool initialize_irrlicht(/* config manager */);
	bool initialize_bullet(/* config manager */);
	void start_loop();

    void attach(IGameObject* obj);
    void detach(IGameObject* obj);

 private:
	std::vector<IGameObject*> event_objs;
};

 

