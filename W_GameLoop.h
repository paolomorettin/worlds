
#pragma once



#include <irrlicht.h>
#include <vector>
#include <memory>

#include "W_GameObject.h"

using namespace irr;

class GameLoop {
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


//void GameLoop::do_frame() {
	
// while( true )
// {
//     int elapsed_time = getcurrenttime() - oldtime;
//     if(elapsed_time >= minimum_elapsed_time)
//       break;
//     sleep(1); //yield cpu to other game thread or processes...
// }


//}

