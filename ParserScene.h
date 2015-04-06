#pragma once
#include"SceneCamera.h"
#include"SceneObject.h"
#include"SceneLight.h"
#include"SceneTreeNode.h"
#include"Vector.h"
#include"SceneContentManager.h"

#include<string>
#include<fstream>
#include<exception>
using namespace std;

class ParserScene
{
public:
	// read a scene file from path, and return its Scene object 
	static SceneTreeNode parseScene(string sceneFileName, vector<SceneObject> *objectStore, SceneContentManager *contentManager);

	//Exceptions
	class ParsingFileException: public exception
	{
	public:
		ParsingFileException()
		{
			this->error = "parsingFileException";
		}
		ParsingFileException(string error)
		{
			this->error = error;
		}
		~ParsingFileException() throw()
		{
		}

		virtual const char* what() const throw()
		{
			return error.c_str();
		}
	private:
		string error;
	};


private:
	//parse a camera structure
	static SceneCamera parseSceneCamera(ifstream &fstream, bool &whantToBeActiveOUT, SceneContentManager *contentManager);
	//parse a transform structure
	static SceneTreeNode parseSceneTransform(ifstream &fstream, string sceneDirectory, vector<SceneObject> *objectStore, SceneContentManager *contentManager, bool isRoot = false);
	//parse a object structure
	static SceneObject parseSceneObject(ifstream &fstream, string sceneDirectory, SceneContentManager *contentManager);
	//parse a light structure
	static SceneLight parseSceneLight(ifstream &fstream);
	
	//read a keyword
	static string readKeyword(ifstream &fstream);
	//read a string value
	static string readStringValue(ifstream &fstream);
	//read a numeric value
	static float readNumericValue(ifstream &fstream);
	//read a Vector value
	static Vector readVectorValue(ifstream &fstream);

	//check open brackets
	static void checkOpenBrackets(ifstream &fstream);
	//check close brackets
	static void checkCloseBrackets(ifstream &fstream);

	//jump the following characters in this line
	static void ignoreCommentLine(ifstream &fstream);

	//check if the string is a number
	static bool isANumber(string str);
};
