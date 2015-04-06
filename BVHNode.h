#pragma once
#include"AABoundingBox.h"
#include"SceneObject.h"
#include<vector>

using namespace std;

class BVHNode :
	public AABoundingBox
{
public:
	BVHNode() { }
	BVHNode(Vector vertex1, Vector vertex2)
		: AABoundingBox(vertex1, vertex2) { }
	BVHNode(vector<AABoundingBox> AABBList)
		: AABoundingBox(AABBList) { }
	BVHNode(vector<Vector> pointList)
		: AABoundingBox(pointList) { }

	vector<unsigned int> getObjectsIDsInSubTree() const;

	bool isLeaf;
	vector<BVHNode> childNodes;
	vector<unsigned int> childObjectIDs;
};
