#include "SceneTreeNode.h"

//utility
template <typename T>
bool tryToRemoveElementFromVector(vector<T> &v, unsigned int index);

//constructors
SceneTreeNode::SceneTreeNode(vector<SceneObject> *objectStore)
{
	parentTransform = 0;

	this->objectStore = objectStore;

	cameraCounter = 0;
	lightCounter = 0;

	indexActiveCamera = -1;
}
SceneTreeNode::SceneTreeNode(const SceneTreeNode &sceneTreeNode)
{
	*this = sceneTreeNode;

	//update parent pointers
	for(vector<SceneTreeNode>::iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
		i->parentTransform = this;
}

//add/remove elements
unsigned int SceneTreeNode::addTreeNode(SceneTreeNode childTreeNode)
{
	//check if keep an eventualy active camera or disable it
	if(childTreeNode.thereIsActiveCameraInSubTree())
	{
		//check if this tree has already an active camera
		bool treeHasActiveCamera = false;
		SceneTreeNode *node = this;
		while (!treeHasActiveCamera && node != 0)
		{
			if(node->thereIsActiveCameraInSubTree())
				treeHasActiveCamera = true;
			else
				node = node->parentTransform;
		}

		if(treeHasActiveCamera) //if this tree has already an active camera
			childTreeNode.deactivateCamera();
	}

	//push transform
	childTreeNode.parentTransform = this;
	childTreeNodes.push_back(childTreeNode);

	//refresh indices
	refreshCounters(OBJECT + CAMERA + LIGHT);

	return childTreeNodes.size() - 1; //index of childTransform
}
unsigned int SceneTreeNode::addObject(SceneObject childObject)
{
	objectStore->push_back(childObject);
	childObjectsIDs.push_back(objectStore->size() - 1); //add index of childObject in objectStore

	refreshCounters(OBJECT);

	return childObjectsIDs.size() - 1;
}
unsigned int SceneTreeNode::addCamera(SceneCamera childCamera)
{
	childCameras.push_back(childCamera);
	
	refreshCounters(CAMERA);

	return childCameras.size() - 1;
}
unsigned int SceneTreeNode::addLight(SceneLight childLight)
{
	childLights.push_back(childLight);
	
	refreshCounters(LIGHT);

	return childLights.size() - 1;
}
bool SceneTreeNode::removeTreeNode(unsigned int index)
{
	if(childTreeNodes.size() > index &&
		!childTreeNodes[index].thereIsActiveCameraInSubTree() &&
		tryToRemoveElementFromVector(childTreeNodes, index))
	{
		refreshCounters(OBJECT + CAMERA + LIGHT);
		return true;
	}
	return false;
}
bool SceneTreeNode::removeObject(unsigned int index)
{
	if(tryToRemoveElementFromVector(childObjectsIDs, index))
	{
		refreshCounters(OBJECT);
		return true;
	}
	return false;
}
bool SceneTreeNode::removeCamera(unsigned int index)
{
	if(indexActiveCamera != index &&
		tryToRemoveElementFromVector(childCameras, index))
	{
		if(index < indexActiveCamera) //if the removed camera is before the active camera
			indexActiveCamera--;

		refreshCounters(CAMERA);
		return true;
	}

	return false;
}
bool SceneTreeNode::removeLight(unsigned int index)
{
	if(tryToRemoveElementFromVector(childLights, index))
	{
		refreshCounters(LIGHT);
		return true;
	}
	return false;
}

//count and get alements in node
unsigned int SceneTreeNode::countSubTreeNodes() const { return childTreeNodes.size(); }
unsigned int SceneTreeNode::countObjectsInNode() const { return childObjectsIDs.size(); }
unsigned int SceneTreeNode::countCamerasInNode() const { return childCameras.size(); }
unsigned int SceneTreeNode::countLightsInNode() const { return childLights.size(); }
const vector<SceneTreeNode> &SceneTreeNode::getSubTreeNodes() const { return childTreeNodes; }
vector<SceneTreeNode> &SceneTreeNode::getSubTreeNodes() { return childTreeNodes; }
const vector<SceneObject> SceneTreeNode::getObjectsInNode() const
{
	vector<SceneObject> objects;

	for(vector<unsigned int>::const_iterator i = childObjectsIDs.begin(); i != childObjectsIDs.end(); i++)
		objects.push_back((*objectStore)[*i]);

	return objects;
}
const vector<unsigned int> &SceneTreeNode::getObjectsIDsInNode() const { return childObjectsIDs; }
const vector<SceneCamera> &SceneTreeNode::getCamerasInNode() const { return childCameras; }
const vector<SceneLight> &SceneTreeNode::getLightsInNode() const { return childLights; }

//count and get elements in sub-tree
unsigned int SceneTreeNode::countObjectsInSubTree() const { return containedObjectsIDs.size(); }
unsigned int SceneTreeNode::countCamerasInSubTree() const { return cameraCounter; }
unsigned int SceneTreeNode::countLightsInSubTree() const { return lightCounter; }
const vector<SceneObject> SceneTreeNode::getObjectsInSubTree() const
{
	vector<SceneObject> objects;

	for(vector<unsigned int>::const_iterator i = containedObjectsIDs.begin(); i != containedObjectsIDs.end(); i++)
		objects.push_back((*objectStore)[*i]);

	return objects;
}
const vector<unsigned int> &SceneTreeNode::getObjectsIDsInSubTree() const { return containedObjectsIDs; }
const vector<SceneCamera> SceneTreeNode::getCamerasInSubTree() const
{
	vector<SceneCamera> cameraList;

	cameraList.insert(cameraList.end(), childCameras.begin(), childCameras.end()); //append child cameras

	for(vector<SceneTreeNode>::const_iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
		if(i->countCamerasInSubTree() > 0)
		{
			vector<SceneCamera> childCameraList = i->getCamerasInSubTree();
			cameraList.insert(cameraList.end(), childCameraList.begin(), childCameraList.end());
		}

	return cameraList;
}
const vector<SceneLight> SceneTreeNode::getLightsInSubTree() const
{
	vector<SceneLight> lightList;

	lightList.insert(lightList.end(), childLights.begin(), childLights.end()); //append child lights

	for(vector<SceneTreeNode>::const_iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
		if(i->countLightsInSubTree() > 0)
		{
			vector<SceneLight> childLightList = i->getLightsInSubTree();
			lightList.insert(lightList.end(), childLightList.begin(), childLightList.end());
		}

	return lightList;
}

unsigned int SceneTreeNode::countTotalPoligonsInSubTree() const
{
	unsigned int counter = 0;
	vector<unsigned int> objectsIDs = getObjectsIDsInSubTree();
	for(vector<unsigned int>::const_iterator i = objectsIDs.begin(); i != objectsIDs.end(); i++)
		counter += (*objectStore)[*i].getFaceCount()/3;

	return counter;
}

//get transformed
const vector<SceneCamera> SceneTreeNode::getTransformedCamerasInSubTree() const
{
	vector<SceneCamera> cameras;

	//add cameras in this node
	cameras = childCameras;

	for(vector<SceneTreeNode>::const_iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
		if(i->countCamerasInSubTree() > 0) //if there are cameras
		{
			vector<SceneCamera> childCameras = i->getTransformedCamerasInSubTree();
			cameras.insert(cameras.end(), childCameras.begin(), childCameras.end()); //add cameras to list
		}

	//transform lights
	for(vector<SceneCamera>::iterator i = cameras.begin(); i != cameras.end(); i++)
		*i = *this * *i;

	return cameras;
}
const vector<SceneLight> SceneTreeNode::getTransformedLightsInSubTree() const
{
	vector<SceneLight> lights;

	//add lights in this node
	lights = childLights;

	for(vector<SceneTreeNode>::const_iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
		if(i->countLightsInSubTree() > 0) //if there are lights
		{
			vector<SceneLight> childLights = i->getTransformedLightsInSubTree();
			lights.insert(lights.end(), childLights.begin(), childLights.end()); //add lights to list
		}

	//transform lights
	for(vector<SceneLight>::iterator i = lights.begin(); i != lights.end(); i++)
		*i = *this * *i;

	return lights;
}
const SceneCamera SceneTreeNode::getTransformedActiveCameraInTree() const
{
	if(indexActiveCamera < childCameras.size()) //the active camera is here
		return *this * childCameras[indexActiveCamera];
	else //the camera is in an other node
	{
		//check in child nodes
		for(vector<SceneTreeNode>::const_iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
			if(i->thereIsActiveCameraInSubTree())
				return *this * i->getTransformedActiveCameraInTree();
	}
}

//manage active camera
bool SceneTreeNode::activateCamera(unsigned int index)
{
	if(index >= countCamerasInSubTree())
		return false;

	if(thereIsActiveCameraInSubTree())
		deactivateCamera();

	if(index >= countCamerasInNode()) //if the camera is contained in a child transformation
	{
		int cameraCounter = countCamerasInNode();
		for(vector<SceneTreeNode>::iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
		{
			if(cameraCounter + i->countCamerasInSubTree() > index)
			{
				i->activateCamera(index - cameraCounter);
				break;
			}
			else
				cameraCounter += i->countCamerasInSubTree();
		}
	}

	indexActiveCamera = index;

	if(parentTransform)
		parentTransform->refreshCounters(CAMERA);
	return true;
}
bool SceneTreeNode::thereIsActiveCameraInSubTree() const { return indexActiveCamera >= 0; }
unsigned int SceneTreeNode::getActiveCameraIndexInSubTree() const { return indexActiveCamera; }
SceneCamera *SceneTreeNode::getActiveCameraInTree()
{
	if(thereIsActiveCameraInSubTree()) //if there is the active camera in this sub-tree
	{
		if(indexActiveCamera < countCamerasInNode()) //the active camera is here
			return &(childCameras[indexActiveCamera]);
		else //the camera is in an other node of the sub-tree
		{
			//check in child transformations
			for(vector<SceneTreeNode>::iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
				if(i->thereIsActiveCameraInSubTree())
					return i->getActiveCameraInTree();
		}
	}
	else
	{
		if(parentTransform)
			return parentTransform->getActiveCameraInTree();
		else
			throw SceneCamera::CameraException("active camera doesn't exist");
	}
}
const SceneCamera *SceneTreeNode::getActiveCameraInTree() const
{
	if(thereIsActiveCameraInSubTree()) //if there is the active camera in this sub-tree
	{
		if(indexActiveCamera < countCamerasInNode()) //the active camera is here
			return &(childCameras[indexActiveCamera]);
		else //the camera is in an other node of the sub-tree
		{
			//check in child transformations
			for(vector<SceneTreeNode>::const_iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
				if(i->thereIsActiveCameraInSubTree())
					return i->getActiveCameraInTree();
		}
	}
	else
	{
		if(parentTransform)
			return parentTransform->getActiveCameraInTree();
		else
			throw SceneCamera::CameraException("active camera doesn't exist");
	}
}

//private
//manage structure
void SceneTreeNode::refreshCounters(ENTITY_TYPE types)
{
	//refresh object
	int res = types & OBJECT;
	if((types & OBJECT) > 0)
	{
		containedObjectsIDs.clear();

		for(vector<unsigned int>::iterator i = childObjectsIDs.begin(); i != childObjectsIDs.end(); i++)
			containedObjectsIDs.push_back(*i);

		for(vector<SceneTreeNode>::iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
		{
			vector<unsigned int> subObjects = i->getObjectsIDsInSubTree();
			containedObjectsIDs.insert(containedObjectsIDs.end(), subObjects.begin(), subObjects.end());
		}
	}

	//refresh camera
	res = types & CAMERA;
	if((types & CAMERA) > 0)
	{
		cameraCounter = countCamerasInNode();
		indexActiveCamera = (indexActiveCamera < countCamerasInNode() ? indexActiveCamera : -1);
		/* There are 4 cases:
		 * 1) there isn't an active camera:
		 *        indexActiveCamera == -1
		 * 2) the active camera is in the "childCameras" vector:
		 *        indexActiveCamera < childCameras.size()
		 *        and I won't re-set it
		 * 3) the active camera is child of a "childTreeNodes"'s branch
		 *        value is wrong, but I will re-set it during the scan
		 * 4) the active camera was child of a "childTreeNodes"'s branch, but this brunch doesn't exist enymore
		 *        value is correct and I wont' re-set it. 
		 */

		for(vector<SceneTreeNode>::iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
		{
			if(i->thereIsActiveCameraInSubTree())
				indexActiveCamera = cameraCounter + i->getActiveCameraIndexInSubTree();
			cameraCounter += i->countCamerasInSubTree();
		}
	}

	//refresh light
	if((types & LIGHT) > 0)
	{
		lightCounter = countLightsInNode();

		for(vector<SceneTreeNode>::iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
			lightCounter += i->countLightsInSubTree();
	}

	//refresh parent's counters
	if(parentTransform)
		parentTransform->refreshCounters(types);
}
void SceneTreeNode::deactivateCamera()
{
	SceneTreeNode *node = this;
	while(node->parentTransform != 0)
		node = node->parentTransform; //find the root node

	while(node->thereIsActiveCameraInSubTree())
	{
		//deactivate this node
		if(node->getActiveCameraIndexInSubTree() >= node->countCamerasInNode()) //if the camera isn't in this node
			for(vector<SceneTreeNode>::iterator i = childTreeNodes.begin(); i != childTreeNodes.end(); i++)
				if(i->thereIsActiveCameraInSubTree())
				{
					node = &(*i);
					break;
				}

		indexActiveCamera = -1;
	}
}

//utility
template <typename T>
bool tryToRemoveElementFromVector(vector<T> &v, unsigned int index)
{
	if(index >= v.size())
		return false;

	vector<T> newVector;

	for(int i = 0 ; i < v.size(); i++)
		if(i != index)
			newVector.push_back(v[i]);

	v = newVector;

	return true;
}