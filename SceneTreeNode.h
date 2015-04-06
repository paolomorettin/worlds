#pragma once
#include "SceneTransform.h"
#include<vector>

using namespace std;

class SceneTreeNode :
	public SceneTransform
{
public:
	SceneTreeNode(vector<SceneObject> *objectStore);
	SceneTreeNode(const SceneTreeNode &sceneTreeNode);

	//add/remove elements
	unsigned int addTreeNode(SceneTreeNode childTreeNode); //return the index of the element for this sub-tree
	unsigned int addObject(SceneObject childObject);
	unsigned int addCamera(SceneCamera childCamera);
	unsigned int addLight(SceneLight childLight);
	bool removeTreeNode(unsigned int index); //remove an element with its index from its list of children. Return true if succeeded.
											//Can't remove a TreeNode if contains the active camera
	bool removeObject(unsigned int index);
	bool removeCamera(unsigned int index); //can't remove active camera
	bool removeLight(unsigned int index);

	//count and get elements in node
	unsigned int countSubTreeNodes() const; //count elements of this type in node
	unsigned int countObjectsInNode() const;
	unsigned int countCamerasInNode() const;
	unsigned int countLightsInNode() const;
	const vector<SceneTreeNode> &getSubTreeNodes() const; //return the list of elements of this type contained in this node
	vector<SceneTreeNode> &getSubTreeNodes();
	const vector<SceneObject> getObjectsInNode() const;
	const vector<unsigned int> &getObjectsIDsInNode() const;
	const vector<SceneCamera> &getCamerasInNode() const;
	const vector<SceneLight> &getLightsInNode() const;
	
	//count and get elements in sub-tree
	unsigned int countObjectsInSubTree() const; //return the number of elements of this type contained in the sub-tree
	unsigned int countCamerasInSubTree() const;
	unsigned int countLightsInSubTree() const;
	const vector<SceneObject> getObjectsInSubTree() const; //return a vector with all contained elements of this type in this Tree
	const vector<unsigned int> &getObjectsIDsInSubTree() const;
	const vector<SceneCamera> getCamerasInSubTree() const;
	const vector<SceneLight> getLightsInSubTree() const;

	unsigned int countTotalPoligonsInSubTree() const;

	//get transformed
	const vector<SceneCamera> getTransformedCamerasInSubTree() const;
	const vector<SceneLight> getTransformedLightsInSubTree() const;
	const SceneCamera getTransformedActiveCameraInTree() const; //return the transformed active camera

	//manage active camera
	bool activateCamera(unsigned int index); //active a new camera with it's index. true if succeeded
	bool thereIsActiveCameraInSubTree() const;
	unsigned int getActiveCameraIndexInSubTree() const; //return -1 if there isn't an active camera in this sub-tree
	SceneCamera *getActiveCameraInTree(); //return a pointer to the current active camera in this Tree.
	const SceneCamera *getActiveCameraInTree() const;

private:
	typedef unsigned int ENTITY_TYPE;
	static const ENTITY_TYPE OBJECT = 1;
	static const ENTITY_TYPE CAMERA = 2;
	static const ENTITY_TYPE LIGHT = 4;

	//manage structure
	void refreshCounters(ENTITY_TYPE types); //update counter and indices, and parent's counter and indices, recursively
	void deactivateCamera(); //deactivate the active camera in the tree

	//node data
	SceneTreeNode *parentTransform;

	//child elements
	vector<SceneTreeNode> childTreeNodes;
	vector<unsigned int> childObjectsIDs;
	vector<SceneCamera> childCameras;
	vector<SceneLight> childLights;

	//container
	vector<SceneObject> *objectStore;
	
	//counters
	vector<unsigned int> containedObjectsIDs;
	unsigned int cameraCounter;
	unsigned int lightCounter;

	//active camera
	int indexActiveCamera;
};

