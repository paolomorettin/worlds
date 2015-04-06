#include "SceneCamera.h"

SceneCamera::SceneCamera(SceneContentManager *contentManager):
	position(0, 0, 0, 1), direction(0, 0, -1, 0), up(0, 1, 0, 0), fovy(90), screenEffectPath("")
{
	this->contentManager = contentManager;
}

//set
void SceneCamera::setName(string name)
{
	this->name = name;
}
void SceneCamera::setPosition(Vector position)
{
	position[3] = 1;
	this->position = position;
}
void SceneCamera::setDirection(Vector direction)
{
	direction[3] = 0;
	this->direction = direction;
}
void SceneCamera::setUp(Vector up)
{
	up[3] = 0;
	this->up = up;
}
void SceneCamera::setFOVy(float fovy)
{
	this->fovy = fovy;
}
void SceneCamera::setScreenEffect(string effect)
{
	screenEffectPath = effect;

	contentManager->loadShader(effect);
}

void SceneCamera::transform(Matrix matrix)
{
	direction = matrix * direction;
	position = matrix * position;
	up = matrix * up;
}

//get
string SceneCamera::getName() const
{
	return name;
}
Vector SceneCamera::getPosition() const
{
	return position;
}
Vector SceneCamera::getDirection() const
{
	return direction;
}
Vector SceneCamera::getUp() const
{
	return up;
}
float SceneCamera::getFOVy() const
{
	return fovy;
}
bool SceneCamera::thereIsScreenEffect() const
{
	return screenEffectPath.length() > 0;
}
Shader SceneCamera::getScreenEffect() const
{
	if(thereIsScreenEffect())
		return contentManager->getShader(screenEffectPath);
	else
		throw CameraException("there isn't a shader");
}
string SceneCamera::getScreenEffectPath() const
{
	return screenEffectPath;
}