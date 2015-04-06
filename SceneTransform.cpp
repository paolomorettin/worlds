#include "SceneTransform.h"
#include<math.h>

SceneTransform::SceneTransform()
{
	translation = Vector();
	angleRotation = 0;
	axisRotation = Vector();
	scale = Vector(1, 1, 1);

}
SceneTransform::SceneTransform(Vector translation, float angleRotation, Vector axisRotation, Vector scale)
{
	this->translation = translation;
	this->angleRotation = angleRotation;
	this->axisRotation = axisRotation;
	this->scale = scale;
}

//Transform properties
//set
void SceneTransform::setTranslation(Vector v) {	translation = v; }
void SceneTransform::setRotation(float angle, Vector axis)
{
	angleRotation = angle;
	axisRotation = axis;
}
void SceneTransform::setScale(Vector v) { scale = v; }

//get
Vector SceneTransform::getTranslation() const {	return translation; }
void SceneTransform::getRotation(float &angleOut, Vector &axisOut) const
{
	angleOut = angleRotation;
	axisOut = axisRotation;
}
Vector SceneTransform::getScale() const { return scale; }
Matrix SceneTransform::getMatrix() const
{
	return Matrix::createTranslation(translation) *
		( Matrix::createRotation(angleRotation, axisRotation) * Matrix::createScale(scale) );
}

//operator
Vector SceneTransform::operator*(Vector v) const { return getMatrix() * v; }
SceneCamera SceneTransform::operator*(SceneCamera camera) const
{
	camera.transform(getMatrix());

	return camera;
}
SceneLight SceneTransform::operator*(SceneLight light) const
{
	light.transform(getMatrix());

	return light;
}