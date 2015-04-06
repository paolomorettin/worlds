#pragma once
#include<string>
#include<GL/glew.h>
#include<GL/glut.h>
#include<exception>

using namespace std;

class Shader
{
public:
	Shader(string path);
	~Shader(void);

	static const int DEFAULT_TANGENT_LOCATION = 10;
	static const int DEFAULT_BITANGENT_LOCATION = 11;

	//get
	const GLuint getVertexShader() const;
	const GLuint getFragmentShader() const;
	const GLuint getProgram() const;

	//Exceptions
	class ShaderLoadException: public exception
	{
	public:
		ShaderLoadException()
		{
			this->error = "can't load this shader";
		}
		ShaderLoadException(string error)
		{
			this->error = error;
		}
		~ShaderLoadException() throw()
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
	void link();
	static void show_info_log(GLuint object, PFNGLGETSHADERIVPROC glGet__iv,
		PFNGLGETSHADERINFOLOGPROC glGet__InfoLog);

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;
};
