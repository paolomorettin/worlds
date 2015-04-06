#include "SceneLight.h"

SceneLight::SceneLight(void)
{
	setPosition(Vector());
	setType(point);
	setIrradiance(Vector());
}
SceneLight::SceneLight(Vector position, LightType type, Vector irradiance)
{
	setPosition(position);
	setType(type);
	setIrradiance(irradiance);
}

//set
void SceneLight::setPosition(Vector position)
{
	position.W = 1;
	this->position = position;
}
void SceneLight::setType(LightType type)
{
	this->type = type;
}
void SceneLight::setIrradiance(Vector rgb)
{
	irradiance.W = 0;
	this->irradiance = rgb;
}

void SceneLight::transform(Matrix matrix)
{
	position = matrix * position;
}

//get
Vector SceneLight::getPosition() const
{
	return position;
}
SceneLight::LightType SceneLight::getType() const
{
	return type;
}
Vector SceneLight::getIrradiance() const
{
	return irradiance;
}
