#include"FreeCameraManager.h"
#include<vector>
#include"Matrix.h"

#define CONST_MOVEMENT 1
#define CONST_ROTATION 0.25

FreeCameraManager::FreeCameraManager(SceneCamera *camera):
	CameraManager(camera)
{
}
FreeCameraManager::~FreeCameraManager(void)
{
}

void FreeCameraManager::moveCamera(InputControl input)
{
	std::vector<unsigned char> pressedKey = input.getPressedKey();
	int deltaMouse[2];
	input.getDeltaMousePosition(deltaMouse[0], deltaMouse[1]);

	//process keyboard input
	int frontMove = 0, lateralMove = 0;
	for(int i = 0; i < pressedKey.size(); i++)
		switch(pressedKey[i])
	{
		case 'w': //go ahead
		case 'W':
			frontMove++;
			break;
		case 's': //go back
		case 'S':
			frontMove--;
			break;
		case 'a': //move left
		case 'A':
			lateralMove--;
			break;
		case 'd': //move right
		case 'D':
			lateralMove++;
			break;
		case 'j': //move camera left
		case 'J':
			deltaMouse[0]--;
			break;
		case 'l': //move camera right
		case 'L':
			deltaMouse[0]++;
			break;
		case 'k': //move camera down
		case 'K':
			deltaMouse[1]--;
			break;
		case 'i': //move camera up
		case 'I':
			deltaMouse[1]++;
			break;
	}

	//move camera
	Vector deltaPosition = camera->getDirection().normalized() * CONST_MOVEMENT *
		frontMove; //front move
	deltaPosition += (camera->getDirection() * camera->getUp()).normalized() * CONST_MOVEMENT *
		lateralMove; //lateral move
	camera->setPosition(camera->getPosition()+deltaPosition);

	//rotate camera
	Matrix rotation = Matrix::createRotationY(deltaMouse[0] * CONST_ROTATION) * //left and right
		Matrix::createRotation(deltaMouse[1] * CONST_ROTATION, camera->getDirection() * camera->getUp()); //up and down
	camera->setDirection(rotation * camera->getDirection());
	camera->setUp(rotation * camera->getUp());
}
string FreeCameraManager::getCameraControlTypeString()
{
	return "FreeCamera";
}