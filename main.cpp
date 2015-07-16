
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

    u32 width = 1240;
    u32 height = 780;
    u32 color_depth = 32;
    bool fullscreen = true;
    bool stencilbuffer = false;
    bool vsync = false;
    video::SColor bg_color = video::SColor(255,50,50,50);

	GameLoop loop;
	loop.initialize_irrlicht();

    loop.smgr->setAmbientLight(video::SColor(100,1,1,50));

    MainGameScene game_scene;
	game_scene.initialize(loop);



    /*
    // TESTS
    int scale = 1;
    scene::IMeshSceneNode * node = smgr -> addCubeSceneNode(2*scale,0,ID_General);
    scene::IAnimatedMeshSceneNode * node2 = smgr -> addAnimatedMeshSceneNode(smgr -> getMesh("./media/cube.3ds"),0,ID_General);
    //node -> setPosition(core::vector3df(1.1*scale,0,0));
    //node2 -> setPosition(core::vector3df(-1.1*scale,0,0));
    //node2 -> setScale(core::vector3df(scale));
    node2->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);

    node -> setDebugDataVisible(scene::EDS_FULL);
    node2 -> setDebugDataVisible(scene::EDS_FULL);
    */

    // hide the cursor
    loop.device->getCursorControl() -> setVisible(false);
  
    const f32 MOVEMENT_SPEED = 700.f;
    u32 then = loop.device -> getTimer() -> getTime();
  
  
    // TESTING TIMER
    W_Timer myTimer = W_Timer(loop.device);
    int previous_sec = 0;
    gui::IGUIStaticText* timer_text = loop.guienv -> addStaticText(L"0.00", rect<s32>(10,10,260,50),false,false,0);
    gui::IGUIFont* font = loop.guienv->getFont("./media/bigfont.png"); 
    timer_text -> setOverrideColor(video::SColor(255,255,255,255));
    if (font){
      timer_text -> setOverrideFont(font);
      printf("HEYY!\n");
    }
    
    int game_state = 0;
    
    while(loop.device -> run())
	{
	    if (loop.device -> isWindowActive() )
		{
		    const u32 now = loop.device -> getTimer() -> getTime();
		    const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		    then = now;

		    // RAY COLLISION (see tutorial 7)
		    core::line3d<f32> ray;
		    ray.start = game_scene.camera->getPosition();
		    ray.end = ray.start + (game_scene.camera->getTarget() - ray.start).normalize() * 0.01f;

		    scene::ISceneNode* collided = loop.cmgr -> getSceneNodeFromRayBB(ray,ID_StartEnd);
		    if (collided){

				if ((collided -> getID() & ID_Start) && (game_state == 0)){
					printf("Collided with ID: %i\n",collided -> getID());
					game_state = 1;
					myTimer.start();
					printf("START!\n");
				} else if ((collided -> getID() & ID_End) && (game_state == 1)){
					printf("Collided with ID: %i\n",collided -> getID());
					myTimer.pause();
					printf("DONE! Time: %i ms\n",myTimer.getTime());
					game_state = 2;
				}
		    }
	  

		    loop.driver->beginScene(true, true, bg_color);
		    loop.smgr->drawAll();
		    loop.guienv->drawAll();
		    loop.driver->endScene();
		    // only update fps display if fps changed
		    int time = myTimer.getTime() / 10;
		    if (previous_sec != time){
		      core::stringw str = L"";     
		      str += ((float)time)/100.0f;
		      timer_text->setText(str.c_str());
		      previous_sec = time;
		    }

		    
		    
		}
	    else 
		loop.device -> yield();
	}
  
    loop.device -> drop();
    return 0;
}
