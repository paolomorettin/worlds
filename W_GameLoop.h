
#pragma once



#include <irrlicht.h>
#include <vector>
#include <memory>

#include "W_GameObject.h"

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

	bool initialize_irrlicht(/* config manager */);
	void start_loop();
};

 

