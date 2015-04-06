#include"Scene.h"
#include"AABoundingBox.h"
#include"ParserScene.h"
#include <algorithm>

const string Scene::DEFAULT_NUMBEROFLIGHTS_VARIABLE_NAME = "gry_numberOfLights";
const string Scene::DEFAULT_TANGENT_VARIABLE_NAME = "gry_tangent";
const string Scene::DEFAULT_BITANGENT_VARIABLE_NAME = "gry_bitangent";
const string Scene::DEFAULT_SECONDPASS_WIDTH_VARIABLE_NAME = "gry_width";
const string Scene::DEFAULT_SECONDPASS_HEIGHT_VARIABLE_NAME = "gry_height";
const string Scene::DEFAULT_SECONDPASS_TEXTURE_VARIABLE_NAME = "gry_texture";

Scene::Scene(string sceneFileName, bool generateBVH) :
	rootTreeNode(ParserScene::parseScene(sceneFileName, &objectStore, &contentManager))
{
	if(generateBVH)
		BVH = new BoundingVolumeHierarchy(rootTreeNode);
	else
		BVH = 0;

	//check camera exists
	if(rootTreeNode.countCamerasInSubTree() == 0)
		throw SceneCamera::CameraException("No camera in scene");

	//if there isn't an active camera, activate the "first"
	if(!rootTreeNode.thereIsActiveCameraInSubTree())
		rootTreeNode.activateCamera(0);
}
Scene::Scene(const Scene &scene) :
	objectStore(scene.objectStore),
	contentManager(scene.contentManager),
	rootTreeNode(scene.rootTreeNode)
{
	if(scene.BVH == 0)
		BVH = 0;
	else
		BVH = new BoundingVolumeHierarchy(*scene.BVH);
}
Scene::~Scene()
{
	if(BVH != 0)
		delete BVH;
}

//get tree
const SceneTreeNode Scene::getTree(float FOVy, float ratio, float nearPlane, float farPlane) const
{
	if(BVH != 0)
	{
		SceneTreeNode newSceneTreeNode(rootTreeNode); //create a copy of the scene tree
		SceneCamera transformedActiveCamera = getTransformedActiveCamera();

		//generate info
		Matrix view = Matrix::createView(transformedActiveCamera.getPosition(), transformedActiveCamera.getDirection(), transformedActiveCamera.getUp());
		
		float right, top;
		top = tan(FOVy/2) * nearPlane;
		right = top * ratio;

		Matrix projection = Matrix::createProjection(-right, -top, nearPlane, right, top, farPlane);

		//select IDs
		vector<unsigned int> selectedIDs = cull(BVH->getBVHRoot(), projection*view);

		//drop tree branch
		sort(selectedIDs.begin(), selectedIDs.end());
		cutTree(newSceneTreeNode, selectedIDs);

		return newSceneTreeNode;
	}

	return rootTreeNode;
}

//count elements
unsigned int Scene::countObjects() const
{
	return rootTreeNode.countObjectsInSubTree();
}
unsigned int Scene::countCameras() const
{
	return rootTreeNode.countCamerasInSubTree();
}
unsigned int Scene::countLights() const
{
	return rootTreeNode.countLightsInSubTree();
}

//get lists
const vector<SceneObject> Scene::getObjectList() const
{
	return rootTreeNode.getObjectsInSubTree();
}
const vector<SceneCamera> Scene::getCameraList() const
{
	return rootTreeNode.getCamerasInSubTree();
}
const vector<SceneLight> Scene::getLightList() const
{
	return rootTreeNode.getLightsInSubTree();
}
//transformed lists
const vector<SceneCamera> Scene::getTransformedCameraList() const
{
	return rootTreeNode.getTransformedCamerasInSubTree();
}
const vector<SceneLight> Scene::getTransformedLightList() const
{
	return rootTreeNode.getTransformedLightsInSubTree();
}

//manage active camera
bool Scene::activateCamera(unsigned int index)
{
	return rootTreeNode.activateCamera(index);
}
const SceneCamera Scene::getTransformedActiveCamera() const
{
	return rootTreeNode.getTransformedActiveCameraInTree();
}
int Scene::getActiveCameraIndex() const
{
	return rootTreeNode.getActiveCameraIndexInSubTree();
}
SceneCamera *Scene::getActiveCamera()
{
	return rootTreeNode.getActiveCameraInTree();
}
const SceneCamera *Scene::getActiveCamera() const
{
	return rootTreeNode.getActiveCameraInTree();
}

//scene info
AABoundingBox Scene::getSceneAABB() const
{
	return AABoundingBox(getObjectsCenters(rootTreeNode));
}
Vector Scene::getCenter() const
{
	return getSceneAABB().getCenter();
}

//private functions
vector<Vector> Scene::getObjectsCenters(SceneTreeNode treeNode)
{
	vector<Vector> objCenters;
	Matrix transformMatrix = treeNode.getMatrix();

	vector<SceneObject> objectsInNode = treeNode.getObjectsInNode();
	for(vector<SceneObject>::const_iterator i = objectsInNode.begin(); i != objectsInNode.end(); i++)
		objCenters.push_back(transformMatrix * i->getCenter());

	vector<SceneTreeNode> subTreeNodes = treeNode.getSubTreeNodes();
	for(vector<SceneTreeNode>::const_iterator i = subTreeNodes.begin(); i != subTreeNodes.end(); i++)
	{
		vector<Vector> childrenObjCenters = getObjectsCenters(*i);
		for(vector<Vector>::const_iterator j = childrenObjCenters.begin();
			j != childrenObjCenters.end(); j++)
			objCenters.push_back(transformMatrix * (*j));
	}

	return objCenters;
}
//return -1 extern, 0 collide, 1 intern
int Scene::checkAABBAndFrustumCollision(const AABoundingBox AABB, Matrix view_projection)
{
	vector<Vector> vertices = AABB.getAllVertices();
	for(vector<Vector>::iterator i = vertices.begin(); i != vertices.end(); i++) //foreach vertex
	{
		*i = view_projection * *i;
		i->normalizeW();
	}

	bool thereIsCollision = false;

	//check all planes
	for(int i = 0; i < 6; i++)
	{
		bool allInside = true, allOutside = true;
		for(vector<Vector>::iterator j = vertices.begin(); j != vertices.end() && (allInside || allOutside); j++) //foreach vertex
		{
			bool inside;
			switch(i) //select plane
			{
			case 0: inside = (j->X >= -1); break; //left
			case 1: inside = (j->Y >= -1); break; //bottom
			case 2: inside = (j->Z >= -1); break; //near
			case 3: inside = (j->X <= 1); break; //right
			case 4: inside = (j->Y <= 1); break; //top
			case 5: inside = (j->Z <= 1); break; //far
			}

			if(inside)
				allOutside = false;
			else
				allInside = false;
		}

		if(allOutside)
			return -1;
		if(!allInside)
			thereIsCollision = true;
	}

	if(thereIsCollision)
		return 0;
	else
		return 1;
}
vector<unsigned int> Scene::cull(const BVHNode &BVHnode, Matrix view_projection)
{
	//select objects
	vector<unsigned int> selectedObjectIDs;

	int collisionResult = checkAABBAndFrustumCollision(BVHnode, view_projection); //test this node
	switch(collisionResult)
	{
	case -1: break; //extern
	case 0: //collide
		if(BVHnode.isLeaf)
		{
			vector<unsigned int> subSelectedIDs = BVHnode.getObjectsIDsInSubTree();
			selectedObjectIDs.insert(selectedObjectIDs.end(), subSelectedIDs.begin(), subSelectedIDs.end());
		}
		else
			for(vector<BVHNode>::const_iterator i = BVHnode.childNodes.begin(); i != BVHnode.childNodes.end(); i++)
			{
				vector<unsigned int> subSelectedIDs = cull(*i, view_projection);
				selectedObjectIDs.insert(selectedObjectIDs.end(), subSelectedIDs.begin(), subSelectedIDs.end());
			}
		break;
	case 1: //intern
		vector<unsigned int> subSelectedIDs = BVHnode.getObjectsIDsInSubTree();
		selectedObjectIDs.insert(selectedObjectIDs.end(), subSelectedIDs.begin(), subSelectedIDs.end());
		break;
	}

	return selectedObjectIDs;
}
void Scene::cutTree(SceneTreeNode &treeNode, const vector<unsigned int> &selectedIDs)
{
	//check objects' IDs in node
	for(int i = treeNode.getObjectsIDsInNode().size() - 1; i >= 0; i--) //foreach object in node (back-order)
		if(find(selectedIDs.begin(), selectedIDs.end(), treeNode.getObjectsIDsInNode()[i]) == selectedIDs.end()) //if there isn't the object
			treeNode.removeObject(i); //remove object

	//check sub tree nodes
	for(int i = treeNode.getSubTreeNodes().size() - 1; i >= 0; i--) //foreach branch
	{
		bool containAnID = false; //true if contains at least a selected ID
		bool notContainAllIDs = false; //true if doesn't contain only selected ID -> true if at least an ID isn't selected

		vector<unsigned int> subTreeIDs = treeNode.getSubTreeNodes()[i].getObjectsIDsInSubTree();
		for(vector<unsigned int>::const_iterator j = subTreeIDs.begin(); j != subTreeIDs.end(); j++)
		{
			if(find(selectedIDs.begin(), selectedIDs.end(), *j) == selectedIDs.end()) //if there isn't the ID
				notContainAllIDs = true;
			else
				containAnID = true;

			if(notContainAllIDs && containAnID) //I've already my infos
				break;
		}

		//chose what to do
		if(notContainAllIDs) //if there is something to do
		{
			if(containAnID) //if you can save something
				cutTree(treeNode.getSubTreeNodes()[i], selectedIDs);
			else
				treeNode.removeTreeNode(i);
		}
	}
}
