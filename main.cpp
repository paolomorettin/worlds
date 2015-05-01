
#include <irrlicht.h>
#include <iostream>

#include "W_EventReceiver.h"

using namespace irr;

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

enum
{
    // I use this ISceneNode ID to indicate a scene node that is
    // not pickable by getSceneNodeAndCollisionPointFromRay()
    ID_IsNotPickable = 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be picked by ray selection.
    IDFlag_IsPickable = 1 << 0,

    // I use this flag in ISceneNode IDs to indicate that the
    // scene node can be highlighted.  In this example, the
    // homonids can be highlighted, but the level mesh can't.
    IDFlag_IsHighlightable = 1 << 1
};



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
  IrrlichtDevice *device =
    createDevice(driverType, core::dimension2d<u32>(width, height), color_depth, fullscreen, stencilbuffer, vsync, &receiver);
  
  if (device == 0)
    return 1; // could not create selected driver.
  
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();

  // load the map
  device->getFileSystem() -> addFileArchive("./media/map-20kdm2.pk3");
  scene::IAnimatedMesh* map_mesh = smgr -> getMesh("20kdm2.bsp");
  scene::IMeshSceneNode* map_node = 0;
  
  if (map_mesh)
    map_node = smgr -> addOctreeSceneNode(map_mesh -> getMesh(0), 0, IDFlag_IsPickable);

  scene::ITriangleSelector* selector = 0;
  if (map_node)
    {
      map_node -> setPosition(core::vector3df(-1350,-130,-1400));
      
      selector = smgr -> createOctreeTriangleSelector(map_node -> getMesh(), map_node, 128);
      map_node -> setTriangleSelector(selector);
      // We're not done with this selector yet, so don't drop it.
    }

  // add the camera (FPS-like)
  scene::ICameraSceneNode * camera = smgr -> addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
  core::vector3df cameraPosition = camera -> getPosition();
  camera -> setPosition(core::vector3df(50,50,-60));
  camera -> setTarget(core::vector3df(-70,30,-60));

  // hide the cursor
  device->getCursorControl() -> setVisible(false);

  if (selector)
    {
      scene::ISceneNodeAnimator* anim = smgr->createCollisionResponseAnimator(selector, camera, core::vector3df(30,50,30), core::vector3df(0,-10,0), core::vector3df(0,30,0));
      selector->drop(); // As soon as we're done with the selector, drop it.
      camera->addAnimator(anim);
      anim->drop();  // And likewise, drop the animator when we're done referring to it.
    }

  
    
  
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
	  driver->beginScene(true, true, video::SColor(255,200,200,200));
	  smgr->drawAll();
	  driver->endScene();
	  
	  
	}
      else 
	device -> yield();
    }
  
  device -> drop();
  return 0;
}
