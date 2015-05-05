
#include <irrlicht.h>
#include <iostream>

#include "W_EventReceiver.h"

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
  
  scene::IMeshSceneNode* map_node = smgr -> addCubeSceneNode(10,parent,IDFlag_IsPickable);
  if (!map_node) { return false; }
  scene::ITriangleSelector* selector = smgr -> createOctreeTriangleSelector(map_node -> getMesh(), map_node, 128);
  map_node -> setTriangleSelector(selector);
  metaselector -> addTriangleSelector(selector);

  scene::IMeshSceneNode* map_subnode1 = smgr -> addCubeSceneNode(10,map_node,IDFlag_IsPickable,core::vector3df(10,10,0));
  selector = smgr -> createOctreeTriangleSelector(map_subnode1 -> getMesh(), map_subnode1, 128);
  map_subnode1 -> setTriangleSelector(selector);
  metaselector -> addTriangleSelector(selector);

  
  scene::IMeshSceneNode* map_subnode2 = smgr -> addCubeSceneNode(10,map_node,IDFlag_IsPickable,core::vector3df(20,20,0));
  selector = smgr -> createOctreeTriangleSelector(map_subnode2 -> getMesh(), map_subnode2, 128);
  map_subnode2 -> setTriangleSelector(selector);
  metaselector -> addTriangleSelector(selector);

  // scale and reposition the level
  map_node -> setScale(core::vector3df(100,1,100));
  map_node -> setPosition(core::vector3df(0,-1000,0));

  scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(metaselector, camera, core::vector3df(30,50,30), core::vector3df(0,-10,0), core::vector3df(0,30,0));

  camera->addAnimator(anim);
  selector->drop(); // As soon as we're done with the selector, drop it.
  metaselector -> drop();
  anim->drop();  

  // platform color
  // Set the color texture
  map_node -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/red.png") );
  map_subnode1 -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/red.png") );
  map_subnode2 -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/red.png") );

  map_node -> getMaterial(0).AmbientColor = video::SColor(0,255,0,255);
  //map_node -> getMaterial(0).DiffuseColor = video::SColor(255,0,0,255);
  map_subnode1 -> getMaterial(0).AmbientColor = video::SColor(0,255,0,255);
  //map_subnode1 -> getMaterial(0).DiffuseColor = video::SColor(255,0,0,255);
  map_subnode2 -> getMaterial(0).AmbientColor = video::SColor(0,255,0,255);
  //map_subnode2 -> getMaterial(0).DiffuseColor = video::SColor(255,0,0,255);
  

  map_node -> setMaterialFlag(video::EMF_LIGHTING, true);
  map_subnode1 -> setMaterialFlag(video::EMF_LIGHTING, true);
  map_subnode2 -> setMaterialFlag(video::EMF_LIGHTING, true);

  return true;
  
  
}



int main()
{

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
    case 'a': driverType = video::EDT_OPENGL;   break;
    case 'b': driverType = video::EDT_DIRECT3D9;break;
    case 'c': driverType = video::EDT_DIRECT3D8;break;
    case 'd': driverType = video::EDT_BURNINGSVIDEO;break;
    case 'e': driverType = video::EDT_SOFTWARE; break;
    case 'f': driverType = video::EDT_NULL;     break;
    default: return 1;
    }
  
  
  W_EventReceiver receiver;
  
  u32 width = 1240;
  u32 height = 780;
  u32 color_depth = 32;
  bool fullscreen = true;
  bool stencilbuffer = false;
  bool vsync = false;
  video::SColor bg_color = video::SColor(255,100,200,200);
  IrrlichtDevice *device =
    createDevice(driverType, core::dimension2d<u32>(width, height), color_depth, fullscreen, stencilbuffer, vsync, &receiver);
  
  if (device == 0)
    return 1; // could not create selected driver.
  
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();

  // ambient light

    
  // add the camera (FPS-like)
  scene::ICameraSceneNode * camera = smgr -> addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
  core::vector3df cameraPosition = camera -> getPosition();
  camera -> setPosition(core::vector3df(0,1000,0));
  camera -> setTarget(core::vector3df(0,-1,0));

  
  if (!createLevel(smgr,0,camera))
    return false;
  

  /*
    LIGHT TEST
  scene::IMeshSceneNode * node = smgr -> addCubeSceneNode(10,0,IDFlag_IsPickable);
  node -> setPosition(core::vector3df(0,980,0));
  node -> setMaterialTexture( 0, smgr -> getVideoDriver() -> getTexture("./media/red.png") );
  node -> getMaterial(0).AmbientColor = video::SColor(0,255,0,255);
  node -> getMaterial(0).DiffuseColor = video::SColor(255,0,0,255);
  node -> setMaterialFlag(video::EMF_LIGHTING,true);
  */


  //lights
  smgr -> setAmbientLight(video::SColor(100,100,100,255));
  scene::ILightSceneNode* light1 = smgr -> addLightSceneNode( camera, core::vector3df(0,0,0), video::SColor(100,100,100,255), 20.0f, -1 ); 
  // light1 -> addAnimator

  //  camera -> addChild(light1);

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

	  core::vector3df camerapos = camera -> getPosition();
	  core::vector3df lightpos = light1 -> getPosition();
	  printf("CAMERA: (%f,%f,%f) LIGHT: (%f,%f,%f)\n", camerapos.X,camerapos.Y,camerapos.Z,lightpos.X,lightpos.Y,lightpos.Z);
	  
	  
	}
      else 
	device -> yield();
    }
  
  device -> drop();
  return 0;
}
