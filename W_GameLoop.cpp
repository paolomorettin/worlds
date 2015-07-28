#include "W_GameLoop.h"
#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>

// for debug
#include <iostream>

// a bit crude, but works.
#define PRINTVEC(vec) vec.x()<<","<<vec.y()<<","<<vec.z()



void GameLoop::start_loop() {
	u32 last_frame_time = timer->getTime();
	while(device -> run()) {
		if (device -> isWindowActive() ) {
			const u32 now = timer->getTime();
			u32 advancement = now - last_frame_time;
			last_frame_time = now;
			// don't update more than 1 second. It would be useless at
			// best.  This may happen in some strange situations
			// (Linux CTRL+Z, putting computer in standby, etc...),
			// and in these cases it's better to continue as if
			// nothing happened instead of handling HUGE numbers later
			// on.
			advancement = advancement > 1000? 1000: advancement;

			// Physics steps:
			// - 1st parameter is the advancement in time (in seconds).
			// - 2nd parameter is the max number of internal substeps.
			// - 3rt parameter is the number of physics steps to perform (in Hz).
			// Note that in the intermediate steps the movement is interpolated.
			const int steps = dynamicsWorld->stepSimulation(now/1000.f, 5, 60);
			// TODO: does it really return the number of steps?

			// manual callbacks of "logic_tick" function for each
			// object.
			for(int i = 0; i < steps; i++) {
				btAlignedObjectArray<btCollisionObject*> objs = dynamicsWorld->getCollisionObjectArray();
				for (int j = 0; j < dynamicsWorld->getNumCollisionObjects(); j++) {
					btRigidBody* body = ((btRigidBody*)objs[j]);
					// MY objects have a logic tick callback.
					IGameObject* myobj = dynamic_cast<IGameObject*>(body->getMotionState());
					if(myobj) {
						myobj->logic_tick(*this);
					}
				}
			}

			{ // FIXME: Debug print of all objects...
				btAlignedObjectArray<btCollisionObject*> objs = dynamicsWorld->getCollisionObjectArray();
				for (int j = 0; j < dynamicsWorld->getNumCollisionObjects(); j++) {
					btRigidBody* body = ((btRigidBody*)objs[j]);

					const btVector3& position = body->getCenterOfMassPosition();
					const btVector3& linearvelocity = body->getLinearVelocity();
					const btVector3& angualrvelocity = body->getAngularVelocity();
					btMotionState* mstate = body->getMotionState();
					std::string itemname;
					IGameObject* myobj = dynamic_cast<IGameObject*>(body->getMotionState());
					if(myobj) {
						itemname = myobj->name;
					}
					if (itemname == std::string()) {
						itemname = "[Not an IGameObject]";
					}
					std::cout<<" - '"<<itemname<<"'"<<std::endl;
					std::cout<<"    - pos   : "<<PRINTVEC(position) << std::endl;
					std::cout<<"    - linvel: "<<PRINTVEC(linearvelocity) << std::endl;
					std::cout<<"    - angvel: "<<PRINTVEC(angualrvelocity) << std::endl;

				}
			}

			video::SColor bg_color = video::SColor(255,50,50,50);
			driver->beginScene(true, true, bg_color);
			for (std::shared_ptr<IGameObject>& obj : game_objects) {
				obj->render(*this, advancement);
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
	return true;
}



bool GameLoop::initialize_bullet() {
	this->config = new btDefaultCollisionConfiguration();
	this->dispatcher = new btCollisionDispatcher(config);
	this->overlappingpaircache = new btDbvtBroadphase();
	this->solver = new btSequentialImpulseConstraintSolver();
	this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingpaircache, solver, config);

}
