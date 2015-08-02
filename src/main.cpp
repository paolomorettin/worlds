#include <irrlicht.h>
#include <cstdlib>

#include <iostream>

#include "GameLoop.hpp"
#include "GameScene.hpp"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

int main(int argc, char** argv)
{

    // DEBUG: allow selection of the level from cmdline
    long int level = -1;
    if(argc == 2) {
        if(std::string(argv[1]) == std::string("--help")) {
            std::cout << "Usage: "<<argv[0]<<" [level_id]"<<std::endl;
            std::cout <<std::endl;
            std::cout<<"Arguments:" <<std::endl;
            std::cout<<"  level_id : [optional] a natural number to select the level." <<std::endl;
            return 0;
        } else {
            level = atol(argv[1]);
        }
    }
    // End of cmdline parsing

    
    video::SColor bg_color = video::SColor(255,50,50,50);

    GameLoop loop;
    loop.initialize_irrlicht();
    loop.initialize_bullet();

    // DEBUG: allow selection of the level from cmdline
    if(level != -1) {
        srand(level);
    }
    // End of DEBUG section
    
    loop.smgr->setAmbientLight(video::SColor(100,1,1,50));

    GameScene game_scene;
    game_scene.create_scene(loop);

    // hide the cursor
    loop.device->getCursorControl() -> setVisible(false);

    loop.start_loop();
    return 0;
}
