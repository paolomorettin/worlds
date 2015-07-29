#include "GameLoop.hpp"
#include "GameObject.hpp"
#include "LevelGenerator.hpp"

#include "Player.hpp"

void PlayerGameObj::logic_tick(GameLoop&) {
	// logic tickling :D
	// here you should handle the keyboard input...
}

void PlayerGameObj::render(GameLoop&, float) {
	// all done by irr
}

void PlayerGameObj::initialize(GameLoop& loop) {

    SKeyMap keyMap[10];
    keyMap[0].Action = EKA_MOVE_FORWARD;
    keyMap[0].KeyCode = KEY_UP;
    keyMap[1].Action = EKA_MOVE_FORWARD;
    keyMap[1].KeyCode = KEY_KEY_W;

    keyMap[2].Action = EKA_MOVE_BACKWARD;
    keyMap[2].KeyCode = KEY_DOWN;
    keyMap[3].Action = EKA_MOVE_BACKWARD;
    keyMap[3].KeyCode = KEY_KEY_S;

    keyMap[4].Action = EKA_STRAFE_LEFT;
    keyMap[4].KeyCode = KEY_LEFT;
    keyMap[5].Action = EKA_STRAFE_LEFT;
    keyMap[5].KeyCode = KEY_KEY_A;

    keyMap[6].Action = EKA_STRAFE_RIGHT;
    keyMap[6].KeyCode = KEY_RIGHT;
    keyMap[7].Action = EKA_STRAFE_RIGHT;
    keyMap[7].KeyCode = KEY_KEY_D;

    keyMap[8].Action = EKA_CROUCH;
    keyMap[8].KeyCode = KEY_SHIFT;

    keyMap[9].Action = EKA_JUMP_UP;
    keyMap[9].KeyCode = KEY_SPACE;

    int ID_camera = 0; // <- are you kidding me? no.
    this->camera = loop.smgr -> addCameraSceneNodeFPS(0, 100, 0.3, ID_camera, keyMap, 0, true, 10);
    this->camera->setFarValue(200);
    this->camera->setNearValue(0.01);
    int ID_playerlight = 0;
    this->playerlight = loop.smgr->addLightSceneNode(camera, core::vector3df(0,0,0), video::SColor(255,255,255,255), 3.0f, ID_playerlight);
}
