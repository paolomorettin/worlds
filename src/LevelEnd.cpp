#include "LevelEnd.hpp"
#include "GameLoop.hpp"
#include <iostream>

using namespace irr;
using namespace irr::core;

btRigidBody* LevelEndObj::initialize(GameLoop& loop, const irr::core::vector3df& position) {

    map_node = loop.smgr -> addSphereSceneNode(0.5);
    map_node -> getMaterial(0).DiffuseColor = video::SColor(100,255,100,255);
    map_node -> getMaterial(0).AmbientColor = video::SColor(100,255,100,255);
    map_node -> getMaterial(0).EmissiveColor = video::SColor(100,255,100,255);
    map_node -> getMaterial(0).ColorMaterial = video::ECM_NONE;

    int ID_endnode = 0;
    map_node -> setID(ID_endnode);
    map_node -> setPosition(position);

    int ID_endlight = 0;
    scene::ILightSceneNode * end_light = loop.smgr -> addLightSceneNode( map_node, core::vector3df(0,0,0), video::SColor(255,255,50,255), 10.0f, ID_endlight );

    // "God-ray" volumetric light
    int ID_godray = 0;
    scene::IVolumeLightSceneNode * godray = loop.smgr->addVolumeLightSceneNode(0, ID_godray,
            32,                                // Subdivisions on U axis
            32,                                // Subdivisions on V axis
            video::SColor(0, 150, 20, 150),  // foot color
            video::SColor(0, 0, 0, 0)); // tail color

    if (godray){
        godray -> setScale(core::vector3df(3.0f, 1000.0f, 3.0f));
        godray -> setPosition(position);

        // load textures for animation
        core::array<video::ITexture*> textures;
        for (s32 g=1; g < 8; g++){
            core::stringc tmp;
            tmp = "./media/portal";
            tmp += g;
            tmp += ".bmp";
            video::ITexture* t = loop.smgr -> getVideoDriver() -> getTexture( tmp.c_str() );
            textures.push_back(t);
        }

        // create texture animator
        scene::ISceneNodeAnimator* glow = loop.smgr->createTextureAnimator(textures, 100);

        // add the animator
        godray->addAnimator(glow);

        // drop the animator because it was created with a create() function
        glow->drop();
    }


    int ID_light = 0;
    scene::ILightSceneNode* light2 = loop.smgr -> addLightSceneNode( 0, core::vector3df(1000,1000,1000), video::SColor(255,255,0,0), 50.0f, ID_light );

    // rotation not matters
    btQuaternion init_rotation(btScalar(0),btScalar(0),btScalar(0));
    // initial position of the rigid body.
    btVector3 init_position(btScalar(position.X), btScalar(position.Y), btScalar(position.Z));
    // initialize the rigid body transform.
    btTransform transform(init_rotation, init_position);
    setWorldTransform(transform);

    // model of the player in the physicial world: A BIG ROUND SPHERE: you fat!
    btCollisionShape* sphere = new btSphereShape(0.5);
    // inertia vector:
    // mass of 10 kg.
    const btScalar mass = 10;
    btVector3 inertiavector(0,0,0);
    sphere->calculateLocalInertia(mass, inertiavector);

    // add the rigid body
    rigid_body = new btRigidBody(mass, this, sphere, inertiavector);
    //rigid_body->setActivationState(DISABLE_DEACTIVATION);

    loop.attach(this);

    return rigid_body;

}


void LevelEndObj::notify(const irr::SEvent& evt) {}
