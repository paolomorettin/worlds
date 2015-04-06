#pragma once

#include"SceneTreeNode.h"
#include"BVHNode.h"
#include"SceneObject.h"
#include<vector>

using namespace std;

class BoundingVolumeHierarchy
{
public:
	BoundingVolumeHierarchy(const SceneTreeNode &sceneTree);

	const BVHNode &getBVHRoot();

private:
	static vector<vector<Vector> > getTransformedObjectsVertices(const SceneTreeNode &sceneNode);

	BVHNode generateBVHNode(const vector<unsigned int> &objectIDs, const vector<AABoundingBox> &boundingBoxes) const;

	BVHNode root;
};
