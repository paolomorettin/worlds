#include "RadialCameraManager.h"
#include<math.h>
#include"Matrix.h"

#define CONST_MOVEMENT 1
#define CONST_ROTATION 1
#define PI 3.14159265

RadialCameraManager::RadialCameraManager(SceneCamera *camera, Vector center):
	CameraManager(camera)
{
	this->center = center;
	lookAtCenter();
}
RadialCameraManager::~RadialCameraManager(void)
{
}

void RadialCameraManager::moveCamera(InputControl input)
{
	std::vector<unsigned char> pressedKey = input.getPressedKey();
	int deltaMouse[2];
	input.getDeltaMousePosition(deltaMouse[0], deltaMouse[1]);
	
	Vector newPosition;

	//alter distance
	float distance = (camera->getPosition() - center).length();
	Vector direction = (camera->getPosition() - center).normalized();
	if(direction == Vector())
		direction = Vector(1, 0, 0);

	for(int i = 0; i < pressedKey.size(); i++)
		switch(pressedKey[i])
	{
		case 'w': //go ahead
		case 'W':
			distance -= CONST_MOVEMENT;
			break;
		case 's': //go back
		case 'S':
			distance += CONST_MOVEMENT;
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
	newPosition = direction * distance;

	//alter angle
	Matrix rotationTransform = Matrix::createRotationY(deltaMouse[0] * CONST_ROTATION) *
		Matrix::createRotation(deltaMouse[1] * CONST_ROTATION, camera->getDirection()*camera->getUp());

	//set
	newPosition = rotationTransform * newPosition + center;

	camera->setPosition(newPosition);
	camera->setDirection(rotationTransform * camera->getDirection());
	camera->setUp(rotationTransform * camera->getUp());
}

void RadialCameraManager::lookAtCenter()
{
	Vector cameraDirection = camera->getDirection();
	Vector cameraUp = camera->getUp();

	Vector centerByCamera = center - camera->getPosition(); //the scene center relative to camera
	float centerAngleOnXZPlane = atan2(-centerByCamera[2], centerByCamera[0]) * 180 / PI;
	float cameraLookAtAngleOnXZPlane = atan2(-cameraDirection[2], cameraDirection[0]) * 180 / PI;
	float rotationOnY = centerAngleOnXZPlane - cameraLookAtAngleOnXZPlane;

	Matrix rotation = Matrix::createRotationY(rotationOnY);
	cameraDirection = rotation * cameraDirection;
	cameraUp = rotation * cameraUp;
	
	float centerVerticalAngle = atan2(centerByCamera[1], sqrt(pow(centerByCamera[0], 2) + pow(centerByCamera[2], 2))) * 180 / PI;
	float cameraLookAtVerticalAngle = atan2(cameraDirection[1], sqrt(pow(cameraDirection[0], 2) + pow(cameraDirection[2], 2))) * 180 / PI;
	float verticalRotation = centerVerticalAngle - cameraLookAtVerticalAngle;

	rotation = Matrix::createRotation(verticalRotation, cameraDirection * cameraUp);
	cameraDirection = rotation * cameraDirection;
	cameraUp = rotation * cameraUp;

	camera->setDirection(cameraDirection);
	camera->setUp(cameraUp);
}
string RadialCameraManager::getCameraControlTypeString()
{
	return "RadialCamera";
}