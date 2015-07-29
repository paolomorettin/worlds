#include "LevelEnd.hpp"
#include "GameLoop.hpp"
#include <iostream>

using namespace irr;
using namespace irr::core;

btGhostObject* LevelEndObj::initialize(GameLoop& loop, const irr::core::vector3df& position) {
	
    scene::IMeshSceneNode * end_node = loop.smgr -> addSphereSceneNode(0.5);
    end_node -> getMaterial(0).DiffuseColor = video::SColor(100,255,100,255);
    end_node -> getMaterial(0).AmbientColor = video::SColor(100,255,100,255);
    end_node -> getMaterial(0).EmissiveColor = video::SColor(100,255,100,255);
    end_node -> getMaterial(0).ColorMaterial = video::ECM_NONE;

    int ID_endnode = 0;
    end_node -> setID(ID_endnode);
    end_node -> setPosition(position);

    int ID_endlight = 0;
    scene::ILightSceneNode * end_light = loop.smgr -> addLightSceneNode( end_node, core::vector3df(0,0,0), video::SColor(255,255,50,255), 10.0f, ID_endlight );

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
	
	ghostObject = new btGhostObject();
	ghostObject->setCollisionShape(new btSphereShape(1));
	// rotation not matters
	btQuaternion init_rotation(btScalar(0),btScalar(0),btScalar(0));
	// initial position of the rigid body.
	btVector3 init_position(btScalar(position.X), btScalar(position.Y), btScalar(position.Z));
	// initialize the rigid body transform.
	btTransform transform(init_rotation, init_position);

	ghostObject->setWorldTransform(transform);
	loop.dynamicsWorld->addCollisionObject(ghostObject);
	loop.attach(this);

}

void LevelEndObj::logic_tick(GameLoop&){
	std::cout<<"End collides with "<<ghostObject->getNumOverlappingObjects()<<std::endl;
	
	for(int i = 0; i < ghostObject->getNumOverlappingObjects(); i++) {
		btRigidBody *body = dynamic_cast<btRigidBody *>(ghostObject->getOverlappingObject(i));
		// do whatever you want to do with these pairs of colliding objects
		
		btMotionState* mstate = body->getMotionState();
		std::string itemname;
		IGameObject* myobj = dynamic_cast<IGameObject*>(mstate);
		if(myobj) {
			itemname = myobj->name;
		}
		if (itemname == std::string()) {
			itemname = "[Not an IGameObject]";
		}
		std::cout<<" - '"<<itemname<<"'"<<std::endl;
	}
}
