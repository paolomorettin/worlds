#pragma once
#include<string>
#include<vector>
#include"Vector.h"
#include"Shader.h"
#include<map>
#include"SceneContentManager.h"
#include"AABoundingBox.h"

using namespace std;

class SceneObject
{
public:
	SceneObject(SceneContentManager *contentManager);

	//set
	void setGeometry(string path);
	void setMaterialAlgorithm(string materialAlgorithmPath);
	void addParameter(string name, vector<float> value);
	void addTexture(string name, string path);

	//get
	int getVertexCount() const;
	int getTextureCount() const;
	int getFaceCount() const;
	int getNormalCount() const;

	string getGeometryPath() const;
	Shader getMaterialAlgorithm() const;
	vector<string> getParameterList() const;
	vector<float> getParameterValue(string parameter) const;
	vector<string> getTextureNames() const;
	vector<GLfloat> getTangents() const;
	vector<GLfloat> getBitangents() const;


	GLuint getVertexBuffer() const;
	GLuint getFaceBuffer() const;
	GLuint getNormalBuffer() const;
	GLuint getTextureBuffer() const;
	GLuint getTextureObject(string name = "") const;

	vector<Vector> getVertexList() const;

	Vector getCenter() const; //return the center of the object by its vertexes
	AABoundingBox getAABoundingBox() const;

	bool isTextureSet() const;

private:
	string geometryPath;
	string materialAlgorithmPath;
	map<string, vector<float> > parameters;
	map<string, string> textureNamePathRelation;

	//content manager
	SceneContentManager *contentManager;
};
