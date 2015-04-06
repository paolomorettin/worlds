#include"SceneObject.h"

SceneObject::SceneObject(SceneContentManager *contentManager)
{
	this->contentManager = contentManager;
}

//set
void SceneObject::setGeometry(string path)
{
	geometryPath = path;

	contentManager->loadGeometry(path);
}
void SceneObject::setMaterialAlgorithm(string materialAlgorithmPath)
{
	this->materialAlgorithmPath = materialAlgorithmPath;

	contentManager->loadShader(materialAlgorithmPath);
}
void SceneObject::addParameter(string name, vector<float> value)
{
	parameters[name] = value;
}
void SceneObject::addTexture(string name, string path)
{
	textureNamePathRelation[name] = path;

	contentManager->loadTexture(path);
}

//get
int SceneObject::getVertexCount() const
{
	return contentManager->getGeometry(geometryPath).getVertexCount();
}
int SceneObject::getTextureCount() const
{
	return contentManager->getGeometry(geometryPath).getTextureCount();
}
int SceneObject::getFaceCount() const
{
	return contentManager->getGeometry(geometryPath).getFaceCount();
}
int SceneObject::getNormalCount() const
{
	return contentManager->getGeometry(geometryPath).getNormalCount();
}

string SceneObject::getGeometryPath() const
{
	return geometryPath;
}
Shader SceneObject::getMaterialAlgorithm() const
{
	return contentManager->getShader(materialAlgorithmPath);
}
vector<string> SceneObject::getParameterList() const
{
	vector<string> pList;
	map<string, vector<float> >::const_iterator iterator;

	for(iterator = parameters.begin(); iterator != parameters.end(); iterator++)
		pList.push_back(iterator->first);

	return pList;
}
vector<float> SceneObject::getParameterValue(string parameter) const
{
	vector<float> values;
	if(parameters.find(parameter) != parameters.end())
		values = parameters.find(parameter)->second;

	return values;
}
vector<string> SceneObject::getTextureNames() const
{
	vector<string> nList;
	map<string, string>::const_iterator iterator;

	for(iterator = textureNamePathRelation.begin(); iterator != textureNamePathRelation.end(); iterator++)
		nList.push_back(iterator->first);

	return nList;
}

GLuint SceneObject::getVertexBuffer() const
{
	return contentManager->getGeometry(geometryPath).getVertexBuffer();
}
GLuint SceneObject::getFaceBuffer() const
{
	return contentManager->getGeometry(geometryPath).getFaceBuffer();
}
GLuint SceneObject::getNormalBuffer() const
{
	return contentManager->getGeometry(geometryPath).getNormalBuffer();
}
GLuint SceneObject::getTextureBuffer() const
{
	return contentManager->getGeometry(geometryPath).getTextureBuffer();
}
GLuint SceneObject::getTextureObject(string name) const
{
	string path;
	if(name.length() == 0) //if the string is empty, load the first texture
		path = textureNamePathRelation.begin()->second;
	else
		path = textureNamePathRelation.find(name)->second;
	Texture texture = contentManager->getTexture(path);

	return texture.getTextureObject();
}

vector<Vector> SceneObject::getVertexList() const
{
	return contentManager->getGeometry(geometryPath).getVertexList();
}

Vector SceneObject::getCenter() const
{
	return contentManager->getGeometry(geometryPath).getAABoundingBox().getCenter();
}
AABoundingBox SceneObject::getAABoundingBox() const
{
	return contentManager->getGeometry(geometryPath).getAABoundingBox();
}

bool SceneObject::isTextureSet() const
{
	return (textureNamePathRelation.size() != 0);
}

vector<GLfloat> SceneObject::getTangents() const {
	return contentManager->getGeometry(geometryPath).getTangents();
}

vector<GLfloat> SceneObject::getBitangents()const {
	return contentManager->getGeometry(geometryPath).getBitangents();
}
