#pragma once
#include"SceneTreeNode.h"
#include"SceneCamera.h"
#include"SceneObject.h"
#include<vector>
#include"Vector.h"
#include"BoundingVolumeHierarchy.h"
#include"AABoundingBox.h"
#include"Matrix.h"
#include <string>

using namespace std;

class Scene
{
public:
	Scene(string sceneFileName, bool generateBVH = true);
	Scene(const Scene &scene);
	~Scene();

	//constants

	static const int DEFAULT_TANGENT_LOCATION = 10;
	static const int DEFAULT_BITANGENT_LOCATION = 11;

	static const string DEFAULT_NUMBEROFLIGHTS_VARIABLE_NAME;
	static const string DEFAULT_TANGENT_VARIABLE_NAME;
	static const string DEFAULT_BITANGENT_VARIABLE_NAME;
	static const string DEFAULT_SECONDPASS_WIDTH_VARIABLE_NAME;
	static const string DEFAULT_SECONDPASS_HEIGHT_VARIABLE_NAME;
	static const string DEFAULT_SECONDPASS_TEXTURE_VARIABLE_NAME;

	//get tree
	const SceneTreeNode getTree(float FOVy, float ratio, float nearPlane, float farPlane) const;

	//count elements
	unsigned int countObjects() const;
	unsigned int countCameras() const;
	unsigned int countLights() const;

	//get lists
	const vector<SceneObject> getObjectList() const;
	const vector<SceneCamera> getCameraList() const;
	const vector<SceneLight> getLightList() const;
	//transformed lists
	const vector<SceneCamera> getTransformedCameraList() const;
	const vector<SceneLight> getTransformedLightList() const;

	//manage active camera
	bool activateCamera(unsigned int index);
	const SceneCamera getTransformedActiveCamera() const;
	int getActiveCameraIndex() const;
	SceneCamera *getActiveCamera();
	const SceneCamera *getActiveCamera() const;

	//scene info
	AABoundingBox getSceneAABB() const;
	Vector getCenter() const;

private:
	static vector<Vector> getObjectsCenters(SceneTreeNode treeNode);
	static int checkAABBAndFrustumCollision(const AABoundingBox AABB, Matrix view_projection); //return -1 extern, 0 collide, 1 intern
	static vector<unsigned int> cull(const BVHNode &BVHnode, Matrix view_projection);
	static void cutTree(SceneTreeNode &treeNode, const vector<unsigned int> &selectedIDs);

	vector<SceneObject> objectStore;
	SceneContentManager contentManager;

	SceneTreeNode rootTreeNode;
	BoundingVolumeHierarchy *BVH;
};
