#pragma once
#include<string>
#include<GL/glew.h>
#include<GL/glut.h>
#include<exception>

using namespace std;

class Texture
{
public:
	Texture(string path);
	~Texture();

	//get
	GLuint getTextureObject() const;

	//Exceptions
	class TextureLoadException: public exception
	{
	public:
		TextureLoadException()
		{
			this->error = "can't load this texture";
		}
		TextureLoadException(string error)
		{
			this->error = error;
		}
		~TextureLoadException() throw()
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
	void load(string path);

	GLuint textureObject;
};
