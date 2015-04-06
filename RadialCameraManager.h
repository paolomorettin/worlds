#pragma once
#include"CameraManager.h"
#include"Vector.h"

class RadialCameraManager :
	public CameraManager
{
public:
	RadialCameraManager(SceneCamera *camera, Vector center);
	~RadialCameraManager(void);

	virtual void moveCamera(InputControl input);
	virtual string getCameraControlTypeString();

private:
	void lookAtCenter();

	Vector center;
};

