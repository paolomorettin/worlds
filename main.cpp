
#include <irrlicht.h>
#include <stdlib.h>
#include <iostream>

#include "W_EventReceiver.h"
#include "W_LevelGenerator.h"
#include "W_Structure.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

enum
{
    ID_IsNotPickable = 0,
    IDFlag_IsPickable = 1 << 0,
    IDFlag_IsHighlightable = 1 << 1
};


bool createLevel(scene::ISceneManager * smgr,scene::ISceneNode * parent,scene::ICameraSceneNode * camera) {

  scene::IMetaTriangleSelector * metaselector = smgr -> createMetaTriangleSelector();

  int scale = 150;
  core::vector3d<int> level_size = core::vector3d<int>(50,100,50);
  W_LevelGenerator level = W_LevelGenerator(level_size,1000);

  core::list<W_Structure> * structures = level.getStructures();



  // create the structures
  core::list<W_Structure>::Iterator iterator;
  for (iterator = structures -> begin(); iterator != structures -> end(); iterator++) {
    
    W_Structure current = *iterator;
    core::vector3df position = core::vector3df(current.pos_x, current.pos_y, current.pos_z);
    core::vector3df size = core::vector3df(current.size_x, current.size_y, current.size_z);

    scene::IMeshSceneNode* map_node = smgr -> addCubeSceneNode(1,parent,IDFlag_IsPickable);

    map_node -> setPosition((scale/2 *size) + scale * position);
    map_node -> setScale(scale * size);

    switch (rand() % 3) {
    case 0 :
    map_node -> getMaterial(0).AmbientColor = video::SColor(255,255,0,0);
    map_node -> getMaterial(0).DiffuseColor = video::SColor(100,100,100,255);
    map_node -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/blue.png") );
    break;
    case 1 :
      map_node -> getMaterial(0).AmbientColor = video::SColor(255,0,255,0);
    map_node -> getMaterial(0).DiffuseColor = video::SColor(100,255,100,100);
    map_node -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/red.png") );
    break;
    case 2 :
    map_node -> getMaterial(0).AmbientColor = video::SColor(255,0,0,255);
    map_node -> getMaterial(0).DiffuseColor = video::SColor(100,100,255,100);
    map_node -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/green.png") );
    break;


    }
    map_node -> setMaterialFlag(video::EMF_LIGHTING, false);
    map_node -> getMaterial(0).ColorMaterial = video::ECM_NONE;

    scene::ITriangleSelector* selector = smgr -> createOctreeTriangleSelector(map_node -> getMesh(), map_node, 128);
    map_node -> setTriangleSelector(selector);
    metaselector -> addTriangleSelector(selector);
    selector->drop();

  }

  // create start/end point
  vector3d<int> startcell = level.getStart();
  vector3df sp = vector3df(startcell.X, startcell.Y, startcell.Z);
  /*
  scene::IMeshSceneNode * start_node = smgr -> addSphereSceneNode(2*scale);
  start_node -> setID(ID_IsNotPickable);
  start_node -> setPosition(scale * sp);*/
  camera -> setPosition(scale * sp);
  printf("Starting cell: (%i,%i,%i)\n",startcell.X,startcell.Y,startcell.Z);
  vector3df camerapos = camera -> getPosition();
  printf("Starting point: (%f,%f,%f)\n",camerapos.X,camerapos.Y,camerapos.Z);

  vector3d<int> endcell = level.getEnd();
  vector3df ep = vector3df(endcell.X, endcell.Y, endcell.Z);
  scene::IMeshSceneNode * end_node = smgr -> addSphereSceneNode(2*scale);  
  end_node -> setPosition(scale * ep);

  
    scene::ITerrainSceneNode* terrain = smgr->addTerrainSceneNode(
        "./media/terrain-heightmap.png",
        0,                  // parent node
        -1,                 // node id
        core::vector3df(0.f, 0.f, 0.f),     // position
        core::vector3df(0.f, 0.f, 0.f),     // rotation
        core::vector3df(level_size.X/4, 20.4f, level_size.Z/4),  // scale
        video::SColor ( 255, 255, 255, 255 ),   // vertexColor
        5,                  // maxLOD
        scene::ETPS_17,             // patchSize
        4                   // smoothFactor
        );

    terrain->setMaterialFlag(video::EMF_LIGHTING, false);

    terrain->setMaterialTexture(0,
            smgr -> getVideoDriver() -> getTexture("./media/terrain-texture.png"));
    terrain->setMaterialTexture(1,smgr -> getVideoDriver() -> getTexture("./media/terrain-texture.png"));
    
    terrain->setMaterialType(video::EMT_DETAIL_MAP);

    terrain->scaleTexture(1.0f, 20.0f);

  
      // create triangle selector for the terrain 
    scene::ITriangleSelector* selector2
        = smgr->createTerrainTriangleSelector(terrain, 0);
    terrain->setTriangleSelector(selector2);
	metaselector->addTriangleSelector(selector2);

	
	scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(metaselector, camera, core::vector3df(30,50,30), core::vector3df(0,-10,0), core::vector3df(0,30,0));

  camera->addAnimator(anim);
  
  metaselector -> drop();
  anim->drop();  

  return true;
  
  
}


int main(int argc, char** argv)
{


  W_EventReceiver receiver;
  
  u32 width = 1240;
  u32 height = 780;
  u32 color_depth = 32;
  bool fullscreen = true;
  bool stencilbuffer = false;
  bool vsync = false;
  video::SColor bg_color = video::SColor(255,100,200,200);


  video::E_DRIVER_TYPE driverType = video::EDT_COUNT;
  char choice = 0;
  if (argc > 1) {
	  choice = argv[1][0];
  }
  else {
	  // ask user for driver
	  printf("Please select the driver you want for this example:\n"	\
			 " (a) OpenGL 1.5\n (b) Direct3D 9.0c\n (c) Direct3D 8.1\n"	\
			 " (d) Burning's Software Renderer\n (e) Software Renderer\n"	\
			 " (f) NullDevice\n (otherKey) exit\n\n");

	  std::cin >> choice;
  }

  switch(choice)
    {
    case 'a': driverType = video::EDT_OPENGL; break;
    case 'b': driverType = video::EDT_DIRECT3D9; break;
    case 'c': driverType = video::EDT_DIRECT3D8; break;
    case 'd': driverType = video::EDT_BURNINGSVIDEO; break;
    case 'e': driverType = video::EDT_SOFTWARE; break;
    case 'f': driverType = video::EDT_NULL; break;
    default :
		printf("Invalid driver.\n");
		return 1;
    }

  IrrlichtDevice *device =
    createDevice(driverType, core::dimension2d<u32>(width, height), color_depth, fullscreen, stencilbuffer, vsync, &receiver);

  
  if (device == 0)
    return 1; // could not create selected driver.
  
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();
    
  // add the camera (FPS-like)

  //scene::ICameraSceneNode * camera = smgr -> addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
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
  
  
  scene::ICameraSceneNode* camera = smgr -> addCameraSceneNodeFPS(0, 100, 0.3, -1, keyMap, 10, true, 10);
  camera->setFarValue(20000);
  camera->setNearValue(1);
  
  smgr -> setAmbientLight(video::SColor(100,1,1,50));

  scene::ILightSceneNode* light1 = smgr -> addLightSceneNode( camera, core::vector3df(0,0,0), video::SColor(255,255,255,255), 30.0f, ID_IsNotPickable ); 
  scene::ILightSceneNode* light2 = smgr -> addLightSceneNode( 0, core::vector3df(0,-100,0), video::SColor(255,255,255,255), 30.0f, ID_IsNotPickable ); 
  
  if (!createLevel(smgr,0,camera))
    return false;
  

  /*
  //  LIGHT TEST
  scene::IMeshSceneNode * node = smgr -> addCubeSceneNode(10,0,IDFlag_IsPickable);
  node -> setPosition(core::vector3df(0,0,20));
  node -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/red.png") );
  node -> getMaterial(0).AmbientColor = video::SColor(255,255,0,0);
  node -> getMaterial(0).DiffuseColor = video::SColor(255,0,255,0);
  //node -> getMaterial(0).ColorMaterial = video::ECM_NONE;
  node -> setMaterialFlag(video::EMF_LIGHTING,true);
  */

  // hide the cursor
  device->getCursorControl() -> setVisible(false);
  
  const f32 MOVEMENT_SPEED = 700.f;
  u32 then = device -> getTimer() -> getTime();
  
  while(device -> run())
    {
      if (device -> isWindowActive() ) 
	{
	  const u32 now = device -> getTimer() -> getTime();
	  const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	  then = now;
	  /*
	  if(receiver.IsKeyDown(irr::KEY_KEY_W))
	    cameraPosition.Z += MOVEMENT_SPEED * frameDeltaTime;
	  else if(receiver.IsKeyDown(irr::KEY_KEY_S))
	    cameraPosition.Z -= MOVEMENT_SPEED * frameDeltaTime;
	  
	  if(receiver.IsKeyDown(irr::KEY_KEY_A))
	    cameraPosition.X -= MOVEMENT_SPEED * frameDeltaTime;
	  else if(receiver.IsKeyDown(irr::KEY_KEY_D))
	    cameraPosition.X += MOVEMENT_SPEED * frameDeltaTime;
	  
	  camera -> setPosition(cameraPosition);
	  */
	  driver->beginScene(true, true, bg_color);
	  smgr->drawAll();
	  driver->endScene();
	  
	  //	  core::vector3df pos = light1 -> getAbsolutePosition();
	  //printf("(%f,%f,%f)\n",pos.X,pos.Y,pos.Z);
	  
	}
      else 
	device -> yield();
    }
  
  device -> drop();
  return 0;
}
