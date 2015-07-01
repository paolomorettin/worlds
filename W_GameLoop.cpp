#include "W_GameLoop.h"

void GameLoop::start_loop() {
	u32 last_logic_tick = timer->getTime();
	while(device -> run()) {
		if (device -> isWindowActive() ) {
			const u32 now = timer->getTime();
			int countlimit = 5; // max 5 logic ticks per frame.
			while ((countlimit-- > 0) || (now - last_logic_tick >= 10)) {
				last_logic_tick += 10;
				for (std::shared_ptr<IGameObject>& obj : game_objects) {
					obj->logic_tick(*this);
				}
			}
			if (now - last_logic_tick >= 1000) {
				// We are behind for more than 1 second! Don't
				// try to catch up the lost ticks otherwise it
				// will be too slow.
				last_logic_tick = now;
			}

			video::SColor bg_color = video::SColor(255,50,50,50);
			driver->beginScene(true, true, bg_color);
			for (std::shared_ptr<IGameObject>& obj : game_objects) {
				obj->rendering_loop(*this, (now - last_logic_tick)/10.0);
			}
			smgr -> drawAll();
			guienv -> drawAll();
			driver->endScene();
		}
		else {
			device -> yield();
		}
	}
}


bool GameLoop::initialize_irrlicht() {
  
    u32 width = 1240;
    u32 height = 780;
    u32 color_depth = 32;
    bool fullscreen = true;
    bool stencilbuffer = false;
    bool vsync = false;
    video::SColor bg_color = video::SColor(255,50,50,50);

	// Don't like OpenGL? Fuck you.
    video::E_DRIVER_TYPE driverType = video::EDT_OPENGL;
	
    this->device = createDevice(driverType,
								core::dimension2d<u32>(width, height),
								color_depth,
								fullscreen,
								stencilbuffer,
								vsync,
								this->evrecv);

    if (device == 0)
		return false; // could not create selected driver.
  
    this->driver = device -> getVideoDriver();
    this->guienv = device -> getGUIEnvironment();
    this->smgr = device -> getSceneManager();
	this->timer = device -> getTimer();
    this->cmgr = smgr -> getSceneCollisionManager();
	return true;
}


