
#include <irrlicht.h>
#include <stdlib.h>
#include <iostream>

#include "EventReceiver.hpp"
#include "Structure.hpp"
#include "Timer.hpp"
#include "GameObject.hpp"
#include "GameScene.hpp"
#include "LevelGenerator.hpp"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main(int argc, char** argv)
{
    EventReceiver receiver;

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
