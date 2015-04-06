#pragma once
#include"SceneCamera.h"
#include"InputControl.h"
#include<string>

using namespace std;

class CameraManager
{
public:
	CameraManager(SceneCamera *camera);

	//set
	void setCamera(SceneCamera *camera);

	virtual void moveCamera(InputControl input) = 0;

	//get
	virtual string getCameraControlTypeString() = 0;

protected:
	SceneCamera *camera;
};

