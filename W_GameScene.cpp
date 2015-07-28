#include "W_GameScene.h"
#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
#include "W_LevelGenerator.h"

#include <iostream>
// used in the camera initialization: wtf is the meaning of this?
// used also in collision detection for the start and end
// TODO: CODE DUPLICATION! This is also in main.cpp
enum {
    ID_General = 0,
    ID_StartEnd = 1,
    ID_Start = 1 << 1,
    ID_End = 1 << 2
};
bool MainGameScene::initialize(GameLoop& gameloop) {

	scene::ISceneManager * smgr = gameloop.smgr; // TODO: initialize
	scene::ISceneNode * parent = nullptr;

	// // add the camera (FPS-like)
    // SKeyMap keyMap[10];
    // keyMap[0].Action = EKA_MOVE_FORWARD;
    // keyMap[0].KeyCode = KEY_UP;
    // keyMap[1].Action = EKA_MOVE_FORWARD;
    // keyMap[1].KeyCode = KEY_KEY_W;

    // keyMap[2].Action = EKA_MOVE_BACKWARD;
    // keyMap[2].KeyCode = KEY_DOWN;
    // keyMap[3].Action = EKA_MOVE_BACKWARD;
    // keyMap[3].KeyCode = KEY_KEY_S;

    // keyMap[4].Action = EKA_STRAFE_LEFT;
    // keyMap[4].KeyCode = KEY_LEFT;
    // keyMap[5].Action = EKA_STRAFE_LEFT;
    // keyMap[5].KeyCode = KEY_KEY_A;

    // keyMap[6].Action = EKA_STRAFE_RIGHT;
    // keyMap[6].KeyCode = KEY_RIGHT;
    // keyMap[7].Action = EKA_STRAFE_RIGHT;
    // keyMap[7].KeyCode = KEY_KEY_D;

    // keyMap[8].Action = EKA_CROUCH;
    // keyMap[8].KeyCode = KEY_SHIFT;

    // keyMap[9].Action = EKA_JUMP_UP;
    // keyMap[9].KeyCode = KEY_SPACE;

    //camera = smgr -> addCameraSceneNodeFPS(0, 100, 0.3, ID_General, keyMap, 10, true, 10);

    float base = 100.0;
    float modelsize = 2.0;

    core::vector3d<int> level_size = core::vector3d<int>(50,100,50);
    W_LevelGenerator level = W_LevelGenerator(level_size,1000);

    core::list<W_Structure> * structures = level.getStructures();
    /*
    // START / END COLLISION TESTS
    core::list<W_Structure> * structures = new list<W_Structure>();
    structures -> push_front( W_Structure(0,0,0,10,1,10) );
    structures -> push_front( W_Structure(9,1,9,1,1,1) );
    */



    // create the structures
    core::list<W_Structure>::Iterator iterator;
    for (iterator = structures -> begin(); iterator != structures -> end(); iterator++) {

		W_Structure current = *iterator;
		core::vector3df position = core::vector3df(current.pos_x, current.pos_y, current.pos_z);
		core::vector3df size = core::vector3df(current.size_x, current.size_y, current.size_z);

		scene::IAnimatedMeshSceneNode* map_node = smgr -> addAnimatedMeshSceneNode(smgr -> getMesh("./media/cube.3ds"),parent,ID_General);

		//    map_node -> setPosition((scale/2 *size) + scale * position);
		core::vector3df new_size = (base/modelsize) * size;
		map_node -> setScale(new_size);
		map_node -> setPosition(new_size  + (base * position));


		map_node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);


		switch (rand() % 3) {
		case 0 :
			map_node -> getMaterial(0).AmbientColor = video::SColor(255,255,0,0);
			map_node -> getMaterial(0).DiffuseColor = video::SColor(100,10,10,255);
			map_node -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/blue.png") );
			break;
		case 1 :
			map_node -> getMaterial(0).AmbientColor = video::SColor(255,0,255,0);
			map_node -> getMaterial(0).DiffuseColor = video::SColor(100,255,10,10);
			map_node -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/red.png") );
			break;
		case 2 :
			map_node -> getMaterial(0).AmbientColor = video::SColor(255,0,0,255);
			map_node -> getMaterial(0).DiffuseColor = video::SColor(100,10,255,10);
			map_node -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/green.png") );
			break;
		}
		map_node -> setMaterialFlag(video::EMF_LIGHTING, true);
		//    map_node -> getMaterial(0).ColorMaterial = video::ECM_NONE;
    }

    // create start/end point
    vector3d<int> startcell = level.getStart();
    vector3df sp = core::vector3df(base/2) + base * vector3df(startcell.X, startcell.Y, startcell.Z);

    scene::IMeshSceneNode * start_node = smgr -> addSphereSceneNode(0.5 * base);
    start_node -> getMaterial(0).DiffuseColor = video::SColor(100,0,255,0);
    start_node -> getMaterial(0).AmbientColor = video::SColor(100,0,255,0);
    start_node -> getMaterial(0).EmissiveColor = video::SColor(100,0,255,0);
    start_node -> getMaterial(0).ColorMaterial = video::ECM_NONE;
    start_node -> setID(ID_Start | ID_StartEnd);
    start_node -> setPosition(sp);

    vector3d<int> endcell = level.getEnd();
    vector3df ep = core::vector3df(base/2) + base * vector3df(endcell.X, endcell.Y, endcell.Z);

    scene::IMeshSceneNode * end_node = smgr -> addSphereSceneNode(0.5*base);
    end_node -> getMaterial(0).DiffuseColor = video::SColor(100,255,100,255);
    end_node -> getMaterial(0).AmbientColor = video::SColor(100,255,100,255);
    end_node -> getMaterial(0).EmissiveColor = video::SColor(100,255,100,255);
    end_node -> getMaterial(0).ColorMaterial = video::ECM_NONE;
    end_node -> setID(ID_End | ID_StartEnd);
    end_node -> setPosition(ep);

    scene::ILightSceneNode * end_light = smgr -> addLightSceneNode( end_node, core::vector3df(0,0,0), video::SColor(255,255,50,255), 300.0f, ID_General );

    // "God-ray" volumetric light
    scene::IVolumeLightSceneNode * godray = smgr->addVolumeLightSceneNode(0, ID_General,
																		  32,                                // Subdivisions on U axis
																		  32,                                // Subdivisions on V axis
																		  video::SColor(0, 150, 20, 150),  // foot color
																		  video::SColor(0, 0, 0, 0)); // tail color

    if (godray){
		godray -> setScale(core::vector3df(300.0f, 10000.0f, 300.0f));
		godray -> setPosition(ep);

        // load textures for animation
        core::array<video::ITexture*> textures;
        for (s32 g=1; g < 8; g++){
            core::stringc tmp;
            tmp = "./media/portal";
            tmp += g;
            tmp += ".bmp";
            video::ITexture* t = smgr -> getVideoDriver() -> getTexture( tmp.c_str() );
            textures.push_back(t);
        }

        // create texture animator
        scene::ISceneNodeAnimator* glow = smgr->createTextureAnimator(textures, 100);

        // add the animator
        godray->addAnimator(glow);

        // drop the animator because it was created with a create() function
        glow->drop();
    }


    scene::ILightSceneNode* light2 = smgr -> addLightSceneNode( 0, core::vector3df(1000,1000,1000), video::SColor(255,255,0,0), 500.0f, ID_General );


    // scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
    //     "./media/terrain-heightmap.png",
    //     0,                  // parent node
    //     -1,                 // node id
    //     core::vector3df(0.f, 0.f, 0.f),     // position
    //     core::vector3df(0.f, 0.f, 0.f),     // rotation
    //     core::vector3df(level_size.X/4, 20.4f, level_size.Z/4),  // scale
    //     video::SColor ( 255, 255, 255, 255 ),   // vertexColor
    //     5,                  // maxLOD
    //     scene::ETPS_17,             // patchSize
    //     4                   // smoothFactor
    //     );

    // terrain->setMaterialFlag(video::EMF_LIGHTING, false);

    // terrain->setMaterialTexture(0,
    //         smgr -> getVideoDriver() -> getTexture("./media/terrain-texture.png"));
    // terrain->setMaterialTexture(1,smgr -> getVideoDriver() -> getTexture("./media/terrain-texture.png"));

    // terrain->setMaterialType(video::EMT_DETAIL_MAP);

    // terrain->scaleTexture(1.0f, 20.0f);


    //   // create triangle selector for the terrain
    // scene::ITriangleSelector* selector2
    //     = smgr->createTerrainTriangleSelector(terrain, 0);
    // terrain->setTriangleSelector(selector2);
	// metaselector->addTriangleSelector(selector2);


	{ // player obj test
		playerObj = new PlayerGameObj();
		playerObj->initialize(gameloop);

		std::cout<<"START PLAYER POS:"<<sp.X<<","<<sp.Y<<","<<sp.Z<<","<<std::endl;
		// from euler angles, other constructors should be preferred (I
		// think)
		btQuaternion init_rotation(btScalar(0.01),btScalar(0.01),btScalar(0.01));
		// initial position of the rigid body.
		btVector3 init_position(btScalar(sp.X), btScalar(sp.Y), btScalar(sp.Z));
		// initialize the rigid body transform.
		btTransform transform(init_rotation, init_position);
		// set motion state. TODO: Find better way to do this
		playerObj->setWorldTransform(transform);

		// model of the player in the physicial world: A BIG ROUND SPHERE: you fat!
		btCollisionShape* sphere = new btSphereShape(1);

		// inertia vector.
		btVector3 inertiavector(0.1,0.1,0.1);

		// add the rigid body
		// mass of 80 kg.

		btRigidBody* test = new btRigidBody(1, playerObj, sphere, inertiavector);
		gameloop.dynamicsWorld->addRigidBody(test);

		// just as a test, start with some initial velocity
		test->applyCentralImpulse(btVector3(1, 200, 1));
	}

    return true;
}

void MainGameScene::render(GameLoop&, float) {
}

void MainGameScene::logic_tick(GameLoop&) {
}



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
	this->camera = loop.smgr -> addCameraSceneNodeFPS(0, 100, 0.3, ID_General, keyMap, 0, true, 10);
    this->camera->setFarValue(20000);
    this->camera->setNearValue(1);
    this->playerlight = loop.smgr->addLightSceneNode(camera, core::vector3df(0,0,0), video::SColor(255,255,255,255), 100.0f, ID_General);
}
