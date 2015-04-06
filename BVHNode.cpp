#include "BVHNode.h"

vector<unsigned int> BVHNode::getObjectsIDsInSubTree() const
{
	vector<unsigned int> IDs = childObjectIDs;

	for(vector<BVHNode>::const_iterator i = childNodes.begin(); i != childNodes.end(); i++)
	{
		vector<unsigned int> subIDs = i->getObjectsIDsInSubTree();
		IDs.insert(IDs.end(), subIDs.begin(), subIDs.end());
	}

	return IDs;
}