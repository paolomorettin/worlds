#include"BoundingVolumeHierarchy.h"
#include"AABoundingBox.h"
#include<vector>

using namespace std;

BoundingVolumeHierarchy::BoundingVolumeHierarchy(const SceneTreeNode &sceneTree)
{
	//generate transformed vertices foreach object
	vector<vector<Vector> > objectsVertexList = getTransformedObjectsVertices(sceneTree);
	
	//generate bounding boxes
	vector<AABoundingBox> boundingBoxes;

	for(vector<vector<Vector> >::iterator i = objectsVertexList.begin(); i != objectsVertexList.end(); i++)
		boundingBoxes.push_back(AABoundingBox(*i));

	root = generateBVHNode(sceneTree.getObjectsIDsInSubTree(), boundingBoxes);
}

const BVHNode &BoundingVolumeHierarchy::getBVHRoot()
{
	return root;
}

//private
vector<vector<Vector> > BoundingVolumeHierarchy::getTransformedObjectsVertices(const SceneTreeNode &sceneNode)
{
	vector<vector<Vector> > objectsVertexList;

	vector<SceneObject> childObjects = sceneNode.getObjectsInNode();
	vector<SceneTreeNode> childTreeNodes = sceneNode.getSubTreeNodes();

	//add vertices of objects in this node
	for(vector<SceneObject>::const_iterator i = childObjects.begin(); i != childObjects.end(); i++)
		objectsVertexList.push_back(i->getVertexList());
	
	//in sub nodes
	for(vector<SceneTreeNode>::const_iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
		if(i->countObjectsInSubTree() > 0) //if there are objects
		{
			vector<vector<Vector> > childObjectVertices = getTransformedObjectsVertices(*i);
			objectsVertexList.insert(objectsVertexList.end(), childObjectVertices.begin(), childObjectVertices.end()); //add vertices to list
		}

	//transform vertices
	for(vector<vector<Vector> >::iterator i = objectsVertexList.begin(); i != objectsVertexList.end(); i++) //foreach object
		for(vector<Vector>::iterator j = i->begin(); j != i->end(); j++) //roreach vertex
			*j = sceneNode * *j;

	return objectsVertexList;
}

BVHNode BoundingVolumeHierarchy::generateBVHNode(const vector<unsigned int> &objectIDs, const vector<AABoundingBox> &boundingBoxes) const
{
	BVHNode node(boundingBoxes);

	if(boundingBoxes.size() > 1)
	{
		//prepare split
		enum SplittableAxies {X = 0, Y, Z};
		vector<AABoundingBox> group1AABB, group2AABB;
		vector<unsigned int> group1ObjID, group2ObjID;

		SplittableAxies splitBy[3];

		//set split order to try
		float widths[3] = {node.getXWidth(), node.getYWidth(), node.getZWidth()};
		for(int i = 0; i < 3; i++)
		{
			SplittableAxies longest;
			float longestValue = -1;
			for(int j = 0; j < 3; j++)
				if(widths[j] > longestValue || longestValue == -1)
				{
					longestValue = widths[j];
					longest = (SplittableAxies)j;
				}
			widths[(int)longest] = -1;
			splitBy[i] = longest;
		}

		//try to split
		for(int trySplit = 0; trySplit < 3; trySplit++)
		{
			group1AABB.clear(); group2AABB.clear();
			group1ObjID.clear(); group2ObjID.clear();

			float splitPosition = (node.getMinVertex()[(int)splitBy[trySplit]] + node.getMaxVertex()[(int)splitBy[trySplit]]) / 2;
		
			for(int i = 0; i != boundingBoxes.size(); i++)
				if(boundingBoxes[i].getCenter()[(int)splitBy[trySplit]] <= splitPosition)
				{
					group1AABB.push_back(boundingBoxes[i]);
					group1ObjID.push_back(objectIDs[i]);
				}
				else
				{
					group2AABB.push_back(boundingBoxes[i]);
					group2ObjID.push_back(objectIDs[i]);
				}

			//check if is a good split
			if(group1AABB.size() != 0 && group2AABB.size() != 0) //ok
			{
				node.childNodes.push_back(generateBVHNode(group1ObjID, group1AABB));
				node.childNodes.push_back(generateBVHNode(group2ObjID, group2AABB));
				node.isLeaf = false;

				return node;
			}
		}
	}

	//there isn't a good split or there isn't to split
	node.childObjectIDs = objectIDs;
	node.isLeaf = true;

	return node;
}