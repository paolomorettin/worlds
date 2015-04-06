#include "Geometry.h"
#include "ParserScene.h"
#include <vector>

using namespace std;

Geometry::Geometry(string path)
{
	load(path);
}

Geometry::~Geometry(void)
{
}

//get
int Geometry::getVertexCount() const { return vertexCount; }
int Geometry::getTextureCount() const { return textureCount; }
int Geometry::getFaceCount() const { return faceCount; }
int Geometry::getNormalCount() const { return normalCount; }
GLuint Geometry::getVertexBuffer() const { return vertexBuffer; }
GLuint Geometry::getFaceBuffer() const { return faceBuffer; }
GLuint Geometry::getNormalBuffer() const { return normalBuffer; }
GLuint Geometry::getTextureBuffer() const { return textureBuffer; }

vector<Vector> Geometry::getVertexList() const { return vertexList; }
AABoundingBox Geometry::getAABoundingBox() const { return boundingBox; }

vector<GLfloat> Geometry::getTangents() const { return tangents; }
vector<GLfloat> Geometry::getBitangents() const { return bitangents; }

//private

Vector Geometry::tangent(objLoader* data, int vertex1, int vertex2, int vertex3){

	Vector V1 = Vector(	data->vertexList[vertex1]->e[0],
						data->vertexList[vertex1]->e[1],
						data->vertexList[vertex1]->e[2]);

	Vector V2 = Vector(	data->vertexList[vertex2]->e[0],
						data->vertexList[vertex2]->e[1],
						data->vertexList[vertex2]->e[2]);

	Vector V3 = Vector(	data->vertexList[vertex3]->e[0],
						data->vertexList[vertex3]->e[1],
						data->vertexList[vertex3]->e[2]);


	float s1 = data->textureList[vertex1]->e[0];
	float t1 = data->textureList[vertex1]->e[1];

	float s2 = data->textureList[vertex2]->e[0];
	float t2 = data->textureList[vertex2]->e[1];

	float s3 = data->textureList[vertex3]->e[0];
	float t3 = data->textureList[vertex3]->e[1];

	float u1, v1, u2, v2;
	float tanX, tanY, tanZ;

	Vector Q1 = V2 - V1;
	Vector Q2 = V3 - V1;

	u1 = s2 - s1; v1 = t2 - t1;
	u2 = s3 - s1; v2 = t3 - t1;

	float coeff = 1/(u1*v2 - u2*v1);

	tanX = coeff * ( v2 * Q1.X - v1 * Q2.X );
	tanY = coeff * ( v2 * Q1.Y - v1 * Q2.Y );
	tanZ = coeff * ( v2 * Q1.Z - v1 * Q2.Z );

	return Vector(tanX, tanY, tanZ);
}

Vector Geometry::bitangent(objLoader* data, int vertex1, int vertex2, int vertex3){

	Vector V1 = Vector(	data->vertexList[vertex1]->e[0],
						data->vertexList[vertex1]->e[1],
						data->vertexList[vertex1]->e[2]);

	Vector V2 = Vector(	data->vertexList[vertex2]->e[0],
						data->vertexList[vertex2]->e[1],
						data->vertexList[vertex2]->e[2]);

	Vector V3 = Vector(	data->vertexList[vertex3]->e[0],
						data->vertexList[vertex3]->e[1],
						data->vertexList[vertex3]->e[2]);


	float s1 = data->textureList[vertex1]->e[0];
	float t1 = data->textureList[vertex1]->e[1];

	float s2 = data->textureList[vertex2]->e[0];
	float t2 = data->textureList[vertex2]->e[1];

	float s3 = data->textureList[vertex3]->e[0];
	float t3 = data->textureList[vertex3]->e[1];

	float u1, v1, u2, v2;
	float bitX, bitY, bitZ;

	Vector Q1 = V2 - V1;
	Vector Q2 = V3 - V1;

	u1 = s2 - s1; v1 = t2 - t1;
	u2 = s3 - s1; v2 = t3 - t1;

	float coeff = 1/(u1*v2 - u2*v1);

	bitX = coeff * ( -u2 * Q1.X + u1 * Q2.X );
	bitY = coeff * ( -u2 * Q1.Y + u1 * Q2.Y );
	bitZ = coeff * ( -u2 * Q1.Z + u1 * Q2.Z );

	return Vector(bitX, bitY, bitZ);
}

void Geometry::load(string path)
{
	//load and process vertexBuffer
	objLoader objData;
	objData.load(path.c_str());

	vertexCount=objData.vertexCount;
	textureCount=objData.textureCount;
	normalCount=objData.normalCount;

	vector<GLfloat> vertexVector;
	vector<GLfloat> normalVector;
	vector<GLfloat>textureVector;
	vector<GLuint> faceVector;

	vertexList.clear();

	if (normalCount > 0 && textureCount > 0 && (normalCount!=vertexCount || textureCount != vertexCount)){
		throw ParserScene::ParsingFileException(); //aggiungere descrizione intuitiva
	}

	for(int i = 0; i < vertexCount; i++)
	{

		vertexVector.push_back(objData.vertexList[i]->e[0]);
		vertexVector.push_back(objData.vertexList[i]->e[1]);
		vertexVector.push_back(objData.vertexList[i]->e[2]);

		//build vertexList for bounding box
		vertexList.push_back(Vector(objData.vertexList[i]->e[0],
									objData.vertexList[i]->e[1],
									objData.vertexList[i]->e[2]));
	}
	//create bounding box
	boundingBox.fit(vertexList);

	for(int i=0; i<objData.faceCount; i++){

		if (normalCount > 0 && textureCount > 0){

			//COMPUTE TANGENTS AND BITANGENTS for the first and second vertex of the face
			Vector vec = tangent(&objData, objData.faceList[i]->vertex_index[0],
										   objData.faceList[i]->vertex_index[1],
										   objData.faceList[i]->vertex_index[2]);

			tangents.push_back(vec.X);
			tangents.push_back(vec.Y);
			tangents.push_back(vec.Z);

			vec = bitangent(&objData, objData.faceList[i]->vertex_index[0],
									  objData.faceList[i]->vertex_index[1],
									  objData.faceList[i]->vertex_index[2]);

			bitangents.push_back(vec.X);
			bitangents.push_back(vec.Y);
			bitangents.push_back(vec.Z);

			vec = tangent(&objData, objData.faceList[i]->vertex_index[1],
									objData.faceList[i]->vertex_index[2],
									objData.faceList[i]->vertex_index[0]);

			tangents.push_back(vec.X);
			tangents.push_back(vec.Y);
			tangents.push_back(vec.Z);

			vec = bitangent(&objData, objData.faceList[i]->vertex_index[1],
									  objData.faceList[i]->vertex_index[2],
									  objData.faceList[i]->vertex_index[0]);

			bitangents.push_back(vec.X);
			bitangents.push_back(vec.Y);
			bitangents.push_back(vec.Z);
		}

		for (int j=2; j<objData.faceList[i]->vertex_count; j++){

			faceVector.push_back(objData.faceList[i]->vertex_index[0]);
			faceVector.push_back(objData.faceList[i]->vertex_index[j-1]);
			faceVector.push_back(objData.faceList[i]->vertex_index[j]);

			if (normalCount > 0 && textureCount > 0){

				//COMPUTE TANGENTS AND BITANGENTS for the remaining vertices
				Vector vec = tangent(&objData, objData.faceList[i]->vertex_index[j],
										objData.faceList[i]->vertex_index[0],
										objData.faceList[i]->vertex_index[j-1]);

				tangents.push_back(vec.X);
				tangents.push_back(vec.Y);
				tangents.push_back(vec.Z);

				vec = bitangent(&objData, objData.faceList[i]->vertex_index[j],
										  objData.faceList[i]->vertex_index[0],
										  objData.faceList[i]->vertex_index[j-1]);

				bitangents.push_back(vec.X);
				bitangents.push_back(vec.Y);
				bitangents.push_back(vec.Z);
			}
		}

	}

	faceCount=faceVector.size();

	// sia per le normali che per le coordinate di texture, assumiamo che l'ordine e il numero
	// di queste rispetti quello dei vertici

	for(int i=0; i<normalCount; i++){
		normalVector.push_back(objData.normalList[i]->e[0]);
		normalVector.push_back(objData.normalList[i]->e[1]);
		normalVector.push_back(objData.normalList[i]->e[2]);
	}


	for (int i=0; i<textureCount;i++){
		textureVector.push_back(objData.textureList[i]->e[0]);
		textureVector.push_back(objData.textureList[i]->e[1]);
	}


	vertexBuffer = make_buffer(GL_ARRAY_BUFFER, &vertexVector[0], vertexVector.size()*sizeof(GLfloat));
	faceBuffer = make_buffer(GL_ELEMENT_ARRAY_BUFFER, &faceVector[0], faceVector.size()*sizeof(GLuint));

	if (normalCount>0){
		normalBuffer = make_buffer(GL_ARRAY_BUFFER, &normalVector[0], normalVector.size()*sizeof(GLfloat));
	}
	if (textureCount>0){
		textureBuffer = make_buffer(GL_ARRAY_BUFFER, &textureVector[0], textureVector.size()*sizeof(GLfloat));
	}
}

GLuint Geometry::make_buffer(GLenum target, const void *buffer_data, GLsizei buffer_size) {
    GLuint buffer;

    glGenBuffers(1, &buffer);
    glBindBuffer(target, buffer);
    glBufferData(target, buffer_size, buffer_data, GL_STATIC_DRAW);
    return buffer;
}
