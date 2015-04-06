#pragma once

#include"MappedVector.h"
#include"Texture.h"
#include"Geometry.h"
#include"Shader.h"
#include<string>

using namespace std;

class SceneContentManager
{
public:
	//texture
	void loadTexture(string path); //can except if path is invalid
	Texture getTexture(string path) const; //can except if doesn't find the path key

	//geometry
	void loadGeometry(string path);
	Geometry getGeometry(string path) const;

	//shader
	void loadShader(string path);
	Shader getShader(string path) const;

private:
	MappedVector<Texture, string> textureManager; //map a Texture with its path
	MappedVector<Geometry, string> geometryManager; //map a Geometry with its path
	MappedVector<Shader, string> shaderManager; //map a Shader with its path
};

