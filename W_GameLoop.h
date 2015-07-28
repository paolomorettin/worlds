
#pragma once



#include <irrlicht.h>
#include <vector>
#include <memory>

#include "W_GameObject.h"

class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btBroadphaseInterface;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;

using namespace irr;

class GameLoop {
 public:
	std::vector<std::shared_ptr<IGameObject>> game_objects;

    IrrlichtDevice *device;
    video::IVideoDriver* driver;
    gui::IGUIEnvironment* guienv;
    scene::ISceneManager* smgr;
    scene::ISceneCollisionManager* cmgr;
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
};

 

