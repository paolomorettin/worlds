
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

  int scale = 1;
  core::vector3d<int> level_size = core::vector3d<int>(10,10,10);
  W_LevelGenerator level = W_LevelGenerator(level_size);
  core::list<W_Structure> * structures = level.getStructures();

  core::list<W_Structure>::Iterator iterator;
  for (iterator = structures -> begin(); iterator != structures -> end(); iterator++) {

    
    W_Structure current = *iterator;
    core::vector3df position = core::vector3df(current.pos_x, current.pos_y, current.pos_z);
    core::vector3df size = core::vector3df(current.size_x, current.size_y, current.size_z);

    scene::IMeshSceneNode* map_node = smgr -> addCubeSceneNode(2,parent,IDFlag_IsPickable);
    map_node -> setScale(scale * size);
    map_node -> setPosition(scale/2 *size);
    map_node -> setPosition(scale * position);

    
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
  
  //scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(metaselector, camera, core::vector3df(30,50,30), core::vector3df(0,-10,0), core::vector3df(0,30,0));

  //camera->addAnimator(anim);
  
  metaselector -> drop();
  //anim->drop();  

  /*
  map_node -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/red.png") );
  */

  return true;
  
  
}


int main()
{


  W_EventReceiver receiver;
  
  u32 width = 1240;
  u32 height = 780;
  u32 color_depth = 32;
  bool fullscreen = true;
  bool stencilbuffer = false;
  bool vsync = false;
  video::SColor bg_color = video::SColor(255,100,200,200);


  // ask user for driver    
  video::E_DRIVER_TYPE driverType;
  printf("Please select the driver you want for this example:\n"	\
	 " (a) OpenGL 1.5\n (b) Direct3D 9.0c\n (c) Direct3D 8.1\n"	\
	 " (d) Burning's Software Renderer\n (e) Software Renderer\n"	\
	 " (f) NullDevice\n (otherKey) exit\n\n");
  
  char i;
  std::cin >> i;
  
  switch(i)
    {
    case 'a': driverType = video::EDT_OPENGL; break;
    case 'b': driverType = video::EDT_DIRECT3D9; break;
    case 'c': driverType = video::EDT_DIRECT3D8; break;
    case 'd': driverType = video::EDT_BURNINGSVIDEO; break;
    case 'e': driverType = video::EDT_SOFTWARE; break;
    case 'f': driverType = video::EDT_NULL; break;
    default : return 1;
    }

  IrrlichtDevice *device =
    createDevice(driverType, core::dimension2d<u32>(width, height), color_depth, fullscreen, stencilbuffer, vsync, &receiver);

  
  if (device == 0)
    return 1; // could not create selected driver.
  
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();
    
  // add the camera (FPS-like)
  scene::ICameraSceneNode * camera = smgr -> addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);


  
  smgr -> setAmbientLight(video::SColor(100,1,1,50));

  scene::ILightSceneNode* light1 = smgr -> addLightSceneNode( camera, core::vector3df(0,100,0), video::SColor(255,255,255,255), 10.0f, ID_IsNotPickable ); 
  //scene::ILightSceneNode* light1 = smgr -> addLightSceneNode( 0, core::vector3df(0,-800,0), video::SColor(255,255,255,255), 30.0f, ID_IsNotPickable ); 
  //camera -> setPosition(core::vector3df(0,1000,0));


  
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
