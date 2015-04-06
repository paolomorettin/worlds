#include "Shader.h"
#include "Scene.h"
#include<fstream>
#include<string>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

Shader::Shader(string path)
{
	load(path);
	link();
}
Shader::~Shader(void)
{
}

//get
const GLuint Shader::getVertexShader() const
{
	return vertexShader;
}
const GLuint Shader::getFragmentShader() const
{
	return fragmentShader;
}
const GLuint Shader::getProgram() const
{
	return program;
}

//private
void Shader::load(string path)
{
	//the first cicle is fot the vertex shader, the second is for the fragment shader

	for(int i = 0; i < 2; i++)
	{
		string shaderPath;
		switch(i)
		{
		case 0: shaderPath = path + ".vs"; break;
		case 1: shaderPath = path + ".fs"; break;
		}

		//open stream
		ifstream stream;
		stream.open(shaderPath.c_str(), ifstream::in);
		if(!stream.good())
			throw ShaderLoadException();

		//load shaders
		GLint length;
		GLchar* source;

		string sourceStr;
		while(stream)
		{
			char c = stream.get();
			if((int)c >= 0)
				sourceStr.push_back(c);
		}
		stream.close();
		
		length = sourceStr.length();
		source = new GLchar[length+1];
		for(int j = 0; j < length; j++)
			source[j] = sourceStr[j];
		source[length] = '\0';

		//create shader
		GLuint *shaderP;
		if(i == 0) //vertex shader
		{
			shaderP = &vertexShader;
			*shaderP = glCreateShader(GL_VERTEX_SHADER);
		}
		else //fragment shader
		{
			shaderP = &fragmentShader;
			*shaderP = glCreateShader(GL_FRAGMENT_SHADER);
		}
		glShaderSource(*shaderP, 1, (const GLchar**)&source, &length);
		delete[] source;

		//compile shader
		glCompileShader(*shaderP);
		GLint shader_ok;
		glGetShaderiv(*shaderP, GL_COMPILE_STATUS, &shader_ok);
		if (!shader_ok) //error in compiling
		{
			fprintf(stderr, "Failed to compile %s:\n", shaderPath.c_str());
			show_info_log(*shaderP, glGetShaderiv, glGetShaderInfoLog);
			glDeleteShader(*shaderP);
			throw ShaderLoadException("can't compile this shader");
		}
	}
}
void Shader::link()
{
	GLint program_ok;
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glBindAttribLocation(program, Scene::DEFAULT_TANGENT_LOCATION, Scene::DEFAULT_TANGENT_VARIABLE_NAME.c_str());
	glBindAttribLocation(program, Scene::DEFAULT_BITANGENT_LOCATION, Scene::DEFAULT_BITANGENT_VARIABLE_NAME.c_str());

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok)
	{
		fprintf(stderr, "Failed to link shader program:\n");
		show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
		throw ShaderLoadException("can't link this shader");
	}
}

void Shader::show_info_log(GLuint object, PFNGLGETSHADERIVPROC glGet__iv,
				   PFNGLGETSHADERINFOLOGPROC glGet__InfoLog)
{
	GLint log_length;
	char *log;
	glGet__iv(object, GL_INFO_LOG_LENGTH, &log_length);
	log = (char *) malloc(log_length);
	glGet__InfoLog(object, log_length, NULL, log);
	fprintf(stderr, "%s", log);
	free(log);
}
