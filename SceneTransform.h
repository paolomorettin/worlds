#pragma once

#include"SceneObject.h"
#include"SceneCamera.h"
#include"SceneLight.h"
#include"Vector.h"
#include"Matrix.h"
#include<vector>
using namespace std;

class SceneTransform
{
public:
	SceneTransform();
	SceneTransform(Vector translation, float angleRotation, Vector axisRotation, Vector scale);

	//Transform properties
	//set
	void setTranslation(Vector v);
	void setRotation(float angle, Vector axis);
	void setScale(Vector v);

	//get
	Vector getTranslation() const;
	void getRotation(float &angleOut, Vector &axisOut) const;
	Vector getScale() const;

	Matrix getMatrix() const;

	//operators
	Vector operator*(Vector v) const; //transform a vector
	SceneCamera operator*(SceneCamera camera) const; //transform a Camera
	SceneLight operator*(SceneLight light) const; //transform a Light

private:
	Vector translation;
	float angleRotation;
	Vector axisRotation;
	Vector scale;
};

