#pragma once
#include"Vector.h"
#include"Matrix.h"

class SceneLight
{
public:
	enum LightType {point, directional};
	
	SceneLight();
	SceneLight(Vector position, LightType type, Vector irradiance);

	//set
	void setPosition(Vector position);
	void setType(LightType type);
	void setIrradiance(Vector rgb);

	void transform(Matrix matrix);

	//get
	Vector getPosition() const;
	LightType getType() const;
	Vector getIrradiance() const;

private:
	Vector position;
	LightType type;
	Vector irradiance;
};

