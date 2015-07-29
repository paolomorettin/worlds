#include "W_GameLoop.h"
#include "W_GameObject.h"
#include "W_LevelGenerator.h"

#include "Player.hpp"

void PlayerGameObj::logic_tick(GameLoop&) {
	// logic tickling :D
	// here you should handle the keyboard input...
}

void PlayerGameObj::render(GameLoop&, float) {
	// all done by irr
}

void PlayerGameObj::initialize(GameLoop& loop) {
    SKeyMap keyMap[1]; // sorry, currently disabled.
    // I don't want irr to interfere with bullet positions by moving the graphics object and not the phyisics one.

    int ID_camera = 0; // <- are you kidding me? no.
    this->camera = loop.smgr -> addCameraSceneNodeFPS(0, 100, 0.3, ID_camera, keyMap, 0, true, 10);
    this->camera->setFarValue(200);
    this->camera->setNearValue(0.01);
    this->playerlight = loop.smgr->addLightSceneNode(camera, core::vector3df(0,0,0), video::SColor(255,255,255,255), 3.0f, ID_General);
}
