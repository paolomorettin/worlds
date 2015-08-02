#include "GameLoop.hpp"
#include "IGameObject.hpp"
#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#define DEBUG_PHYSOBJECTS 0

// for debug
#include <iostream>

// a bit crude, but works.
#define PRINTVEC(vec) vec.x()<<","<<vec.y()<<","<<vec.z()

GameLoop::GameLoop() {
    ev_recv = new EventReceiver();
}

GameLoop::~GameLoop() {
    delete ev_recv;
}

void GameLoop::attach(IGameObject* obj) {
    event_objs.push_back(obj);
}

void GameLoop::detach(IGameObject* obj) {
    auto iter = event_objs.begin();
    for (; iter != event_objs.end(); iter++) {
        if (*iter == obj) break;
    }
    if (iter != event_objs.end()) {
        event_objs.erase(iter);
    }
}

void GameLoop::start_loop() {
    u32 last_frame_time = timer->getTime();
    float time_scale = 1; // change for fun effects

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
            const int steps = dynamicsWorld->stepSimulation(now/1000.f*time_scale, 25, 1/300.0);
            // TODO: does it really return the number of steps?



#if DEBUG_PHYSOBJECTS
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
#endif

            video::SColor bg_color = video::SColor(255,50,50,50);
            driver->beginScene(true, true, bg_color);
            smgr -> drawAll();
            guienv -> drawAll();
            driver->endScene();
        } else {
            device -> yield();
        }
    }
}

static void motor_post_tick_callback (btDynamicsWorld *world, btScalar timeStep) {
    GameLoop *loop = static_cast<GameLoop *>(world->getWorldUserInfo());

    loop->handle_collisions();
    
    // manual callbacks of "logic_tick" function for each
    // object.
    for (IGameObject* obj: loop->event_objs) {
        obj->logic_tick(*loop);
    }
}

void GameLoop::handle_collisions() {

    std::cout << "test"<<std::endl;
    int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();
    for (int i=0;i<numManifolds;i++){
        btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
        const btCollisionObject* obA = contactManifold->getBody0();
        const btCollisionObject* obB = contactManifold->getBody1();

        auto gameobjA_it = collision_objs.find(obA);
        auto gameobjB_it = collision_objs.find(obB);
        
        if (gameobjA_it != collision_objs.end()) {
            // is interested in collision notification!
            // (and most importantly it's a real IGameObject)
            // we are not sure if it's a real collision object there.
            std::cout << gameobjA_it->second->name << " collides with "<<obB<<std::endl;
            gameobjA_it->second->collision_callback(obB);
        }
        
        if (gameobjB_it != collision_objs.end()) {
            // is interested in collision notification!
            // (and most importantly it's a real IGameObject)
            // we are not sure if it's a real collision object there.
            std::cout << gameobjB_it->second->name << " collides with "<<obA<<std::endl;
            gameobjB_it->second->collision_callback(obA);
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
            this->ev_recv);

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
    this->dynamicsWorld->setGravity(this->dynamicsWorld->getGravity()/20);
    this->dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
    this->dynamicsWorld->setInternalTickCallback(motor_post_tick_callback,this,true);
}
