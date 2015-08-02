#include "GameScene.hpp"
#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
#include "LevelGenerator.hpp"
#include "LevelEnd.hpp"

#include <iostream>

void MainGameScene::notify(const irr::SEvent& evt) {}
void StaticStructure::notify(const irr::SEvent& evt) {}

bool MainGameScene::initialize(GameLoop& gameloop) {

    scene::ISceneManager * smgr = gameloop.smgr; // TODO: initialize
    scene::ISceneNode * parent = nullptr;

    core::vector3d<int> level_size = core::vector3d<int>(50,100,50);
    LevelGenerator level = LevelGenerator(level_size,1000);

    core::list<Structure> * structures = level.getStructures();

    // create the structures
    for (Structure& current: *structures) {
        StaticStructure* structure = new StaticStructure();
        current.size_x *= world_scale;
        current.size_y *= world_scale;
        current.size_z *= world_scale;
        current.pos_x *= world_scale;
        current.pos_y *= world_scale;
        current.pos_z *= world_scale;
        btRigidBody* rigidb = structure->initialize(gameloop, current);
        gameloop.dynamicsWorld->addRigidBody(rigidb);

    }

    // create start point
    vector3d<int> startcell = level.getStart();
    vector3df sp = core::vector3df(1.0/2) + vector3df(startcell.X, startcell.Y, startcell.Z);
    sp *= world_scale;

    scene::IMeshSceneNode * start_node = smgr -> addSphereSceneNode(0.5);
    start_node -> getMaterial(0).DiffuseColor = video::SColor(100,0,255,0);
    start_node -> getMaterial(0).AmbientColor = video::SColor(100,0,255,0);
    start_node -> getMaterial(0).EmissiveColor = video::SColor(100,0,255,0);
    start_node -> getMaterial(0).ColorMaterial = video::ECM_NONE;
    int ID_startnode = 0;
    start_node -> setID(ID_startnode);
    start_node -> setPosition(sp);


    // create end point

    vector3d<int> endcell = level.getEnd();
    vector3df ep = core::vector3df(1.0/2) + vector3df(endcell.X, endcell.Y, endcell.Z);
    ep *= world_scale;
    LevelEndObj* levelend = new LevelEndObj();
    btGhostObject* end_point_rb = levelend->initialize(gameloop, ep);

    playerObj = new PlayerGameObj();
    btRigidBody* player_rigid_body = playerObj->initialize(gameloop, sp);
    gameloop.dynamicsWorld->addRigidBody(player_rigid_body);
    // just as a test, start with some initial velocity
    player_rigid_body->setActivationState(DISABLE_DEACTIVATION);
    return true;
}

void MainGameScene::render(GameLoop&, float) {
}

void MainGameScene::logic_tick(GameLoop&) {
}




void StaticStructure::logic_tick(GameLoop&) { }

void StaticStructure::render(GameLoop&, float) { }

btRigidBody* StaticStructure::initialize(GameLoop& loop, const Structure& current) {

    core::vector3df position = core::vector3df(current.pos_x, current.pos_y, current.pos_z);
    core::vector3df size = core::vector3df(current.size_x, current.size_y, current.size_z);

    int ID_mapnode;
    scene::IAnimatedMeshSceneNode* map_node = loop.smgr -> addAnimatedMeshSceneNode(loop.smgr -> getMesh("./media/cube.3ds"), nullptr, ID_mapnode);
    map_node -> setScale(size);
    map_node -> setPosition(position);
    map_node->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);


    switch (rand() % 3) {
        case 0 :
            map_node -> getMaterial(0).AmbientColor = video::SColor(255,255,0,0);
            map_node -> getMaterial(0).DiffuseColor = video::SColor(100,10,10,255);
            map_node -> setMaterialTexture( 0, loop.smgr -> getVideoDriver() -> getTexture("./media/blue.png") );
            break;
        case 1 :
            map_node -> getMaterial(0).AmbientColor = video::SColor(255,0,255,0);
            map_node -> getMaterial(0).DiffuseColor = video::SColor(100,255,10,10);
            map_node -> setMaterialTexture( 0, loop.smgr -> getVideoDriver() -> getTexture("./media/red.png") );
            break;
        case 2 :
            map_node -> getMaterial(0).AmbientColor = video::SColor(255,0,0,255);
            map_node -> getMaterial(0).DiffuseColor = video::SColor(100,10,255,10);
            map_node -> setMaterialTexture( 0, loop.smgr -> getVideoDriver() -> getTexture("./media/green.png") );
            break;
    }
    map_node -> setMaterialFlag(video::EMF_LIGHTING, true);
    //    map_node -> getMaterial(0).ColorMaterial = video::ECM_NONE;

    //size /=2;
    position += size/2;
    size /=2;
    btQuaternion init_rotation(btScalar(0),btScalar(0),btScalar(0));
    // initial position of the rigid body.
    btVector3 init_position(btScalar(position.X), btScalar(position.Y), btScalar(position.Z));
    // initialize the rigid body transform.
    btTransform transform(init_rotation, init_position);
    // set motion state.
    this->setWorldTransform(transform);

    // model of the building is a box
    btCollisionShape* sphere = new btBoxShape(btVector3(size.X, size.Y, size.Z));

    // add a static rigid body (mass = 0).
    return new btRigidBody(0, this, sphere);
}
