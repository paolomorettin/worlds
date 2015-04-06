#pragma once

#include"Vector.h"
#include"Matrix.h"
#include"Shader.h"
#include"SceneContentManager.h"
#include<string>
#include<exception>
using namespace std;

class SceneCamera
{
public:
	SceneCamera(SceneContentManager *contentManager);
	
	//set
	void setName(string name);
	void setPosition(Vector position);
	void setDirection(Vector direction);
	void setUp(Vector up);
	void setFOVy(float FOVy);
	void setScreenEffect(string effectPath);

	void transform(Matrix matrix);
	
	//get
	string getName() const;
	Vector getPosition() const;
	Vector getDirection() const;
	Vector getUp() const;
	float getFOVy() const;
	bool thereIsScreenEffect() const;
	Shader getScreenEffect() const;
	string getScreenEffectPath() const;

	//Exceptions
	class CameraException: public exception
	{
	public:
		CameraException() { }
		CameraException(string error) { this->error = error; }
		~CameraException() throw() { }

		virtual const char* what() const throw()
		{
			return error.c_str();
		}
	private:
		string error;
	};

private:
	string name;
	Vector position;
	Vector direction;
	Vector up;
	float fovy;
	string screenEffectPath;

	SceneContentManager *contentManager;
};
