#pragma once

#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include "IGameObject.hpp"
#include <irrlicht.h>

class LevelEndObj: public IGameObject {
irr::scene::IAnimatedMeshSceneNode* map_node;
	btGhostObject *ghostObject;
public:

	explicit LevelEndObj():
		IGameObject("End"), map_node(nullptr)
	{
	}

		virtual btGhostObject* initialize(GameLoop&, const irr::core::vector3df& position);
	virtual void logic_tick(GameLoop&);

	virtual ~LevelEndObj() {};
};

