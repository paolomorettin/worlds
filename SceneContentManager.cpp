#include "SceneContentManager.h"
#include <exception>

//Texture
void SceneContentManager::loadTexture(string path)
{
	if(!textureManager.exists(path))
	{
		Texture texture(path);
		textureManager.addElement(texture, path);
	}
}
Texture SceneContentManager::getTexture(string path) const
{
	return textureManager[path];
}

//Geometry
void SceneContentManager::loadGeometry(string path)
{
	if(!geometryManager.exists(path))
	{
		Geometry geometry(path);
		geometryManager.addElement(geometry, path);
	}
}
Geometry SceneContentManager::getGeometry(string path) const
{
	return geometryManager[path];
}

//shader
void SceneContentManager::loadShader(string path)
{
	if(!shaderManager.exists(path))
	{
		Shader shader(path);
		shaderManager.addElement(shader, path);
	}
}
Shader SceneContentManager::getShader(string path) const
{
	return shaderManager[path];
}
