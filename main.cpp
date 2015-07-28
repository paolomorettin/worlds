
#include <irrlicht.h>
#include <stdlib.h>
#include <iostream>

#include "W_EventReceiver.h"
#include "W_Structure.h"
#include "W_Timer.h"
#include "W_GameObject.h"
#include "W_GameScene.h"
#include "W_LevelGenerator.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif


// used in the camera initialization: wtf is the meaning of this?
// used also in collision detection for the start and end
// TODO: CODE DUPLICATION! This is also in W_GameScene.cpp
enum {
    ID_General = 0,
    ID_StartEnd = 1,
    ID_Start = 1 << 1,
    ID_End = 1 << 2
};

int main(int argc, char** argv)
{
    W_EventReceiver receiver;

    video::SColor bg_color = video::SColor(255,50,50,50);

	GameLoop loop;
	loop.evrecv = &receiver;
	loop.initialize_irrlicht();
	loop.initialize_bullet();

    loop.smgr->setAmbientLight(video::SColor(100,1,1,50));

    MainGameScene game_scene;
	game_scene.initialize(loop);

    // hide the cursor
    loop.device->getCursorControl() -> setVisible(false);
  
	loop.start_loop();
    return 0;
}
