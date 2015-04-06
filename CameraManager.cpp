#include "CameraManager.h"

CameraManager::CameraManager(SceneCamera *camera)
{
	this->camera = camera;
}

//set
void CameraManager::setCamera(SceneCamera *camera)
{
	this->camera = camera;
}
