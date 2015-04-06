#include "AABoundingBox.h"

AABoundingBox::AABoundingBox()
{
	setted = false;
}
AABoundingBox::AABoundingBox(Vector vertex1, Vector vertex2)
{
	setVertices(vertex1, vertex2);
}
AABoundingBox::AABoundingBox(vector<AABoundingBox> AABBList)
{
	fit(AABBList);
}
AABoundingBox::AABoundingBox(vector<Vector> pointList)
{
	fit(pointList);
}

//set
void AABoundingBox::setVertices(Vector vertex1, Vector vertex2)
{
	for(int i = 0; i < 3; i++)
		if(vertex1[i] <= vertex2[i])
		{
			minVertex[i] = vertex1[i];
			maxVertex[i] = vertex2[i];
		}
		else
		{
			minVertex[i] = vertex2[i];
			maxVertex[i] = vertex1[i];
		}
	minVertex[3] = maxVertex[3] = 1;

	setted = true;
}
void AABoundingBox::fit(vector<AABoundingBox> AABBList)
{
	vector<Vector> pointList;
	for(vector<AABoundingBox>::iterator i = AABBList.begin(); i != AABBList.end(); i++)
	{
		pointList.push_back(i->getMinVertex());
		pointList.push_back(i->getMaxVertex());
	}
	fit(pointList);
}
void AABoundingBox::fit(vector<Vector> pointList)
{
	setted = false;

	for(vector<Vector>::iterator iterator = pointList.begin(); iterator != pointList.end(); iterator++)
	{
		for(int i = 0; i < 3; i++)
		{
			if(minVertex[i] > (*iterator)[i] || !setted)
				minVertex[i] = (*iterator)[i];
			if(maxVertex[i] < (*iterator)[i] || !setted)
				maxVertex[i] = (*iterator)[i];
		}
		setted = true;
	}

	minVertex[3] = maxVertex[3] = 1;
}
void AABoundingBox::unset() { setted = false; }

//get
bool AABoundingBox::isSetted() const { return setted; }
Vector AABoundingBox::getMinVertex() const { return (setted ? minVertex: Vector()); }
Vector AABoundingBox::getMaxVertex() const { return (setted ? maxVertex: Vector()); }
Vector AABoundingBox::getCenter() const { return (getMinVertex() + getMaxVertex()) / 2; }
vector<Vector> AABoundingBox::getAllVertices() const
{
	vector<Vector> vertices;
	vertices.push_back(Vector(minVertex[0], minVertex[1], minVertex[2]));
	vertices.push_back(Vector(maxVertex[0], minVertex[1], minVertex[2]));
	vertices.push_back(Vector(minVertex[0], maxVertex[1], minVertex[2]));
	vertices.push_back(Vector(minVertex[0], minVertex[1], maxVertex[2]));
	vertices.push_back(Vector(maxVertex[0], maxVertex[1], minVertex[2]));
	vertices.push_back(Vector(maxVertex[0], minVertex[1], maxVertex[2]));
	vertices.push_back(Vector(minVertex[0], maxVertex[1], maxVertex[2]));
	vertices.push_back(Vector(maxVertex[0], maxVertex[1], maxVertex[2]));
	return vertices;
}

float AABoundingBox::getXWidth() const { return maxVertex.X - minVertex.X; }
float AABoundingBox::getYWidth() const { return maxVertex.Y - minVertex.Y; }
float AABoundingBox::getZWidth() const { return maxVertex.Z - minVertex.Z; }
float AABoundingBox::getVolume() const { return getXWidth() * getYWidth() * getZWidth(); }

bool AABoundingBox::collide(AABoundingBox aabb) const
{
	return this->isSetted() && aabb.isSetted() &&
		this->minVertex.X <= aabb.maxVertex.X && this->maxVertex.X >= aabb.minVertex.X &&
		this->minVertex.Y <= aabb.maxVertex.Y && this->maxVertex.Y >= aabb.minVertex.Y &&
		this->minVertex.Z <= aabb.maxVertex.Z && this->maxVertex.Z >= aabb.minVertex.Z;
}