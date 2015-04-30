/** Example 002 Quake3Map

This Tutorial shows how to load a Quake 3 map into the engine, create a
SceneNode for optimizing the speed of rendering, and how to create a user
controlled camera.

Please note that you should know the basics of the engine before starting this
tutorial. Just take a short look at the first tutorial, if you haven't done
this yet: http://irrlicht.sourceforge.net/tut001.html

Lets start like the HelloWorld example: We include the irrlicht header files
and an additional file to be able to ask the user for a driver type using the
console.
*/
#include <irrlicht.h>
#include <iostream>

#include "W_EventReceiver.h"

using namespace irr;

/*
Again, to be able to use the Irrlicht.DLL file, we need to link with the
Irrlicht.lib. We could set this option in the project settings, but to make it
easy, we use a pragma comment lib:
*/
#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

/*
Ok, lets start. Again, we use the main() method as start, not the WinMain().
*/
int main()
{

	// ask user for driver

	video::E_DRIVER_TYPE driverType;

	printf("Please select the driver you want for this example:\n"\
		" (a) OpenGL 1.5\n (b) Direct3D 9.0c\n (c) Direct3D 8.1\n"\
		" (d) Burning's Software Renderer\n (e) Software Renderer\n"\
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

	IrrlichtDevice *device =
	  createDevice(driverType, core::dimension2d<u32>(800, 600), 32, false, false, false, &receiver);

	if (device == 0)
		return 1; // could not create selected driver.

	video::IVideoDriver* driver = device->getVideoDriver();
	scene::ISceneManager* smgr = device->getSceneManager();

	/*
	To display the Quake 3 map, we first need to load it. Quake 3 maps
	are packed into .pk3 files which are nothing else than .zip files.
	So we add the .pk3 file to our irr::io::IFileSystem. After it was added,
	we are able to read from the files in that archive as if they are
	directly stored on the disk.
	*/
	device->getFileSystem()->addFileArchive("./media/map-20kdm2.pk3");

	/*
	Now we can load the mesh by calling
	irr::scene::ISceneManager::getMesh(). We get a pointer returned to an
	irr::scene::IAnimatedMesh. As you might know, Quake 3 maps are not
	really animated, they are only a huge chunk of static geometry with
	some materials attached. Hence the IAnimatedMesh consists of only one
	frame, so we get the "first frame" of the "animation", which is our
	quake level and create an Octree scene node with it, using
	irr::scene::ISceneManager::addOctreeSceneNode().
	The Octree optimizes the scene a little bit, trying to draw only geometry
	which is currently visible. An alternative to the Octree would be a
	irr::scene::IMeshSceneNode, which would always draw the complete
	geometry of the mesh, without optimization. Try it: Use
	irr::scene::ISceneManager::addMeshSceneNode() instead of
	addOctreeSceneNode() and compare the primitives drawn by the video
	driver. (There is a irr::video::IVideoDriver::getPrimitiveCountDrawn()
	method in the irr::video::IVideoDriver class). Note that this
	optimization with the Octree is only useful when drawing huge meshes
	consisting of lots of geometry.
	*/
	scene::IAnimatedMesh* mesh = smgr->getMesh("20kdm2.bsp");
	scene::ISceneNode* node = 0;

	if (mesh)
	  node = smgr->addOctreeSceneNode(mesh->getMesh(0), 0, -1, 1024);


	/*
	if (node)
	node->setPosition(core::vector3df(-1300,-144,-1249));
	*/

	// add the camera (FPS-like)
	scene::ISceneNode * camera = smgr->addCameraSceneNodeFPS();
	core::vector3df cameraPosition = camera->getPosition();
	// hide the cursor
	device->getCursorControl()->setVisible(false);

	const f32 MOVEMENT_SPEED = 700.f;
	u32 then = device -> getTimer() -> getTime();

	while(device->run())
	  {
	  const u32 now = device->getTimer()->getTime();
	  const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
	  then = now;

	  if(receiver.IsKeyDown(irr::KEY_KEY_W))
            cameraPosition.Z += MOVEMENT_SPEED * frameDeltaTime;
	  else if(receiver.IsKeyDown(irr::KEY_KEY_S))
            cameraPosition.Z -= MOVEMENT_SPEED * frameDeltaTime;

	  if(receiver.IsKeyDown(irr::KEY_KEY_A))
            cameraPosition.X -= MOVEMENT_SPEED * frameDeltaTime;
	  else if(receiver.IsKeyDown(irr::KEY_KEY_D))
            cameraPosition.X += MOVEMENT_SPEED * frameDeltaTime;

	  node->setPosition(cameraPosition);

	  driver->beginScene(true, true, video::SColor(255,200,200,200));
	  smgr->drawAll();
	  driver->endScene();

	  
	  }

	device->drop();
	return 0;
}
