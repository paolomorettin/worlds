#pragma once
#include<string>
#include<GL/glew.h>
#include<GL/glut.h>
#include<exception>
#include"Vector.h"
#include"objParser/objLoader.h"
#include"AABoundingBox.h"

using namespace std;

class Geometry
{
public:
	Geometry(string path);
	~Geometry(void);

	//get
	int getVertexCount() const;
	int getTextureCount() const;
	int getFaceCount() const;
	int getNormalCount() const;
	GLuint getVertexBuffer() const;
	GLuint getFaceBuffer() const;
	GLuint getNormalBuffer() const;
	GLuint getTextureBuffer() const;

	vector<Vector> getVertexList() const;
	AABoundingBox getAABoundingBox() const;

	vector<GLfloat> getTangents() const;
	vector<GLfloat> getBitangents() const;

	//Exceptions
	class GeometryLoadException: public exception
	{
	public:
		GeometryLoadException()
		{
			this->error = "can't load this geometry";
		}
		GeometryLoadException(string error)
		{
			this->error = error;
		}
		~GeometryLoadException() throw()
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
	Vector tangent(objLoader* data, int vertex1, int vertex2, int vertex3);
	Vector bitangent(objLoader* data, int vertex1, int vertex2, int vertex3);

	void load(string path);
	GLuint make_buffer(GLenum target, const void *buffer_data, GLsizei buffer_size);
	

	int vertexCount;
	int textureCount;
	int faceCount;
	int normalCount;
	GLuint vertexBuffer;
	GLuint faceBuffer;
	GLuint normalBuffer;
	GLuint textureBuffer;

	vector<Vector> vertexList;
	AABoundingBox boundingBox;

	vector<GLfloat> tangents;
	vector<GLfloat> bitangents;
};
