#include "ParserScene.h"
#include <sstream>
using namespace std;

#define EXCEPT_FILE_OPENING_MSG "Can't open the scene file"
#define EXCEPT_FILE_READING_MSG "Error reading file"
#define EXCEPT_FILE_SYNTAX_MSG "Wrong scene file syntax"

int cameraCounter;

SceneTreeNode ParserScene::parseScene(string sceneFileName, vector<SceneObject> *objectStore, SceneContentManager *contentManager)
{
	SceneTreeNode sceneTree(objectStore);

	//separate directory and file name
	int lastSeparatorIndex = sceneFileName.find_last_of("/\\");
	string sceneDirectory;
	string sceneName(sceneFileName);

	if(lastSeparatorIndex >= 0)
	{
		sceneDirectory = sceneFileName.substr(0, lastSeparatorIndex+1);
		sceneName = sceneFileName.substr(lastSeparatorIndex+1);
	}

	//create a stream from file
	ifstream fstream;
	fstream.open((sceneDirectory + sceneName).c_str(), ifstream::in);

	//initialize counter
	cameraCounter = 0;

	//read the root transform in file
	if(fstream.good())
		sceneTree = parseSceneTransform(fstream, sceneDirectory, objectStore, contentManager, true);
	else
		throw ParserScene::ParsingFileException(EXCEPT_FILE_OPENING_MSG);

	//close file stream
	fstream.close();

	return sceneTree;
}

SceneCamera ParserScene::parseSceneCamera(ifstream &fstream, bool &whantToBeActiveOUT, SceneContentManager *contentManager)
{
	SceneCamera camera(contentManager);
	whantToBeActiveOUT = false;

	//check the open bracket at starting of the camera zone
	checkOpenBrackets(fstream);

	while(!fstream.eof())
	{
		string key = readKeyword(fstream);

		if(key.compare("active") == 0) //is active?
		{
			whantToBeActiveOUT = readNumericValue(fstream) == 1; //false otherwise
		}
		else if(key.compare("position") == 0) //read position
		{
			Vector position = readVectorValue(fstream);
			camera.setPosition(position);
		}
		else if(key.compare("direction") == 0) //read direction
		{
			Vector direction = readVectorValue(fstream);
			camera.setDirection(direction);
		}
		else if(key.compare("up") == 0) //read the up vector
		{
			Vector up = readVectorValue(fstream);
			camera.setUp(up);
		}
		else if(key.compare("FOVy") == 0) //read the FOVy value
		{
			float FOVy = readNumericValue(fstream);
			camera.setFOVy(FOVy);
		}
		else if(key.compare("screenEffect") == 0) //read the screen effect
		{
			string effect = readStringValue(fstream);
			camera.setScreenEffect(effect);
		}
		else if(key.compare("name") == 0) //read the camera name
		{
			string name = readStringValue(fstream);
			camera.setName(name);
		}
		else if(key.length() > 0 && key[0]=='#') //it's a comment
			ignoreCommentLine(fstream);

		else if(key.compare("}") == 0) //the camera zone is finished
			break;
		
		else
			throw ParserScene::ParsingFileException(EXCEPT_FILE_SYNTAX_MSG);
	}

	//if there isn't, assign a name to the camera
	if(camera.getName().length() == 0)
	{
		stringstream sstr;
		sstr << "camera" << cameraCounter;
		camera.setName(sstr.str());
	}

	cameraCounter++;

	return camera;
}
SceneTreeNode ParserScene::parseSceneTransform(ifstream &fstream, string sceneDirectory, vector<SceneObject> *objectStore, SceneContentManager *contentManager, bool isRoot)
{
	SceneTreeNode transform(objectStore);

	//check the open bracket at starting of the transform zone
	if(!isRoot)
		checkOpenBrackets(fstream);

	while(!fstream.eof())
	{
		string key = readKeyword(fstream);

		if(key.compare("translation") == 0 && !isRoot) //read translation
		{
			Vector translation = readVectorValue(fstream);
			transform.setTranslation(translation);
		}
		else if(key.compare("rotation") == 0 && !isRoot) //read rotation
		{
			float angle = readNumericValue(fstream);
			Vector axis = readVectorValue(fstream);
			transform.setRotation(angle, axis);
		}
		else if(key.compare("scale") == 0 && !isRoot) //read scale
		{
			Vector scale = readVectorValue(fstream);
			transform.setScale(scale);
		}
		else if(key.compare("Transform") == 0) //read a sub Transform
		{
			SceneTreeNode subTransform = parseSceneTransform(fstream, sceneDirectory, objectStore, contentManager);
			transform.addTreeNode(subTransform);
		}
		else if(key.compare("Object") == 0) //read an Object
		{
			SceneObject object = parseSceneObject(fstream, sceneDirectory, contentManager);
			transform.addObject(object);
		}
		else if(key.compare("Camera") == 0) //read a Camera
		{
			bool whantToBeActive;
			unsigned int cameraIndex;
			SceneCamera camera = parseSceneCamera(fstream, whantToBeActive, contentManager);
			cameraIndex = transform.addCamera(camera);

			if(whantToBeActive && !transform.thereIsActiveCameraInSubTree())
				transform.activateCamera(cameraIndex);
		}
		else if(key.compare("Light") == 0) //read a Light
		{
			SceneLight light = parseSceneLight(fstream);
			transform.addLight(light);
		}
		else if(key.length() > 0 && key[0]=='#') //it's a comment
			ignoreCommentLine(fstream);

		else if(key.compare("}") == 0) //the transform zone is finished
			break;
		
		else if (key.length() > 0 && key.compare("\\n")!=0)
			throw ParserScene::ParsingFileException(EXCEPT_FILE_SYNTAX_MSG);
	}

	return transform;
}
SceneObject ParserScene::parseSceneObject(ifstream &fstream, string sceneDirectory, SceneContentManager *contentManager)
{
	SceneObject object(contentManager);

	//check the open bracket at starting of the object zone
	checkOpenBrackets(fstream);

	while(!fstream.eof())
	{
		string key = readKeyword(fstream);

		if(key.compare("geometry") == 0) //read the geometry path
		{
			string geometry = readStringValue(fstream);
			object.setGeometry(sceneDirectory + geometry);
		}
		else if(key.compare("material") == 0) //read the material shader
		{
			string material = readStringValue(fstream);
			object.setMaterialAlgorithm(material);
		}
		else if(key.compare("params") == 0) //read params
		{
			string params = readStringValue(fstream);
			istringstream paramStream(params);

			//read params
			enum ParamState{name, equal, value};
			ParamState paramState = name;

			string pname;
			vector<float> pValues;

			while(!paramStream.eof())
			{
				string pkey;
				try
				{
					paramStream >> pkey;
				}
				catch (exception)
				{
					throw ParserScene::ParsingFileException(EXCEPT_FILE_READING_MSG);
				}

				if(pkey.length() > 0)
					switch(paramState)
					{
					case name: //read the parameter name
						pname = pkey;
						paramState = equal;
						break;

					case equal: //we expect an equal
						if(pkey.compare("=") != 0)
							throw ParserScene::ParsingFileException(EXCEPT_FILE_READING_MSG);
						else
							paramState = value;
						break;

					case value: //this can be a value (float)
						if(isANumber(pkey)) //add this value in the list of values
						{
							istringstream keyStream(pkey);

							float keyValue;
							keyStream >> keyValue;

							pValues.push_back(keyValue);
						}
						else //is an other parameter's name
						{
							//unget the pkey
							for(int i = 0; i < pkey.length(); i++)
								paramStream.unget();

							object.addParameter(pname, pValues);
							pValues.clear();
							paramState = name;
						}
						break;
					}
			}
			if(paramState == value && pValues.size() > 0)
				object.addParameter(pname, pValues);
			else if(paramState != name)
				throw ParserScene::ParsingFileException(EXCEPT_FILE_READING_MSG);
		}
		else if(key.length() >= 7 && //read the texture path
			key.substr(0, 7).compare("texture") == 0)
		{
			string name = readStringValue(fstream);
			string path = readStringValue(fstream);

			object.addTexture(name, sceneDirectory + path);
		}
		else if(key.length() > 0 && key[0]=='#') //it's a comment
			ignoreCommentLine(fstream);

		else if(key.compare("}") == 0) //the object zone is finished
			break;
		
		else
			throw ParserScene::ParsingFileException(EXCEPT_FILE_SYNTAX_MSG);
	}

	return object;
}
SceneLight ParserScene::parseSceneLight(ifstream &fstream)
{
	SceneLight light;

	checkOpenBrackets(fstream);
	while(!fstream.eof())
	{
		string key = readKeyword(fstream);

		if(key.compare("position") == 0) //read the light position
		{
			Vector position = readVectorValue(fstream);
			int type = readNumericValue(fstream);

			light.setPosition(position);
			//light.setType((type == 0 ? SceneLight::LightType::directional : SceneLight::LightType::point));
		}
		else if(key.compare("irradiance") == 0) //read the irradiance
		{
			Vector irradiance = readVectorValue(fstream);

			light.setIrradiance(irradiance);
		}
		else if(key.length() > 0 && key[0]=='#') //it's a comment
			ignoreCommentLine(fstream);

		else if(key.compare("}") == 0) //the object zone is finished
			break;
		
		else
			throw ParserScene::ParsingFileException(EXCEPT_FILE_SYNTAX_MSG);
	}

	return light;
}


string ParserScene::readKeyword(ifstream &fstream)
{
	string keyword;

	try
	{
		fstream >> keyword;
	}
	catch (exception)
	{
		throw ParserScene::ParsingFileException(EXCEPT_FILE_READING_MSG);
	}

	return keyword;
}
string ParserScene::readStringValue(ifstream &fstream)
{
	enum StringScanState {notStarted, inside, finished};

	StringScanState state = notStarted;
	string str;
	char c = '\0';
	
	while(!fstream.eof() && state != finished)
	{
		try
		{
			fstream.get(c);
		}
		catch (exception)
		{
			throw ParserScene::ParsingFileException(EXCEPT_FILE_READING_MSG);
		}

		switch(state)
		{
		case notStarted:
			if(c == '"')
				state = inside;
			else if(c != ' ' && c != '\t')
				throw ParserScene::ParsingFileException(EXCEPT_FILE_SYNTAX_MSG);
			break;

		case inside:
			if(c != '"')
				str += c;
			else
				state = finished;
		}
	}

	if(state != finished)
		throw ParserScene::ParsingFileException(EXCEPT_FILE_SYNTAX_MSG);

	return str;
}
float ParserScene::readNumericValue(ifstream &fstream)
{
	float number;
	
	try
	{
		fstream >> number;
	}
	catch (exception)
	{
		throw ParserScene::ParsingFileException(EXCEPT_FILE_READING_MSG);
	}

	return number;
}
Vector ParserScene::readVectorValue(ifstream &fstream)
{
	Vector vector;
	
	vector[0] = readNumericValue(fstream);
	vector[1] = readNumericValue(fstream);
	vector[2] = readNumericValue(fstream);

	return vector;
}
void ParserScene::checkOpenBrackets(ifstream &fstream)
{
	string bracket;
	
	try
	{
		fstream >> bracket;
	}
	catch (exception)
	{
		throw ParserScene::ParsingFileException(EXCEPT_FILE_READING_MSG);
	}
	
	if(bracket.compare("{") != 0)
		throw ParserScene::ParsingFileException(EXCEPT_FILE_SYNTAX_MSG);
}
void ParserScene::checkCloseBrackets(ifstream &fstream)
{
	string bracket;
	
	try
	{
		fstream >> bracket;
	}
	catch (exception)
	{
		throw ParserScene::ParsingFileException(EXCEPT_FILE_READING_MSG);
	}
	
	if(bracket.compare("}") != 0)
		throw ParserScene::ParsingFileException(EXCEPT_FILE_SYNTAX_MSG);
}

void ParserScene::ignoreCommentLine(ifstream &fstream)
{
	char c = '\0';
	while(c != '\n' && !fstream.eof())
		try
		{
			fstream.get(c);
		}
		catch (exception)
		{
			throw ParserScene::ParsingFileException(EXCEPT_FILE_READING_MSG);
		}
}

bool ParserScene::isANumber(string str)
{
	istringstream inpStream(str);
	double inpValue;
	if (inpStream >> inpValue)
		return true;
	else
		return false;
}
