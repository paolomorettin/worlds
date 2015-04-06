#pragma once
#include"Vector.h"
#include<vector>

using namespace std;

class AABoundingBox
{
public:
	AABoundingBox();
	AABoundingBox(Vector vertex1, Vector vertex2);
	AABoundingBox(vector<AABoundingBox> AABBList);
	AABoundingBox(vector<Vector> pointList);

	//set
	void setVertices(Vector vertex1, Vector vertex2);
	void fit(vector<AABoundingBox> AABBList);
	void fit(vector<Vector> pointList);
	void unset();

	//get
	bool isSetted() const;
	Vector getMinVertex() const;
	Vector getMaxVertex() const;
	Vector getCenter() const;
	vector<Vector> getAllVertices() const;

	float getXWidth() const;
	float getYWidth() const;
	float getZWidth() const;
	float getVolume() const;

	bool collide(AABoundingBox aabb) const;

private:
	bool setted;
	Vector minVertex, maxVertex;
};

