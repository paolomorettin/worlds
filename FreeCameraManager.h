#pragma once
#include "CameraManager.h"

class FreeCameraManager :
	public CameraManager
{
public:
	FreeCameraManager(SceneCamera *camera);
	~FreeCameraManager(void);

	virtual void moveCamera(InputControl input);
	virtual string getCameraControlTypeString();
};
