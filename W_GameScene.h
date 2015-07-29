#ifndef W_GAME_SCENE_H
#define W_GAME_SCENE_H

#include "W_GameLoop.h"
#include "W_GameObject.h"
#include "W_LevelGenerator.h"

class PlayerGameObj;

class MainGameScene : public IGameObject {
 public:

	PlayerGameObj* playerObj = nullptr;

	virtual void render(GameLoop&, float);
	virtual void logic_tick(GameLoop&);

    bool initialize(GameLoop& loop);
	
	virtual ~MainGameScene() {};
};


class PlayerGameObj: public IGameObject {
	scene::ICameraSceneNode* camera; // irr camera
	scene::ILightSceneNode* playerlight; // irr light that follows the player.

public:

	explicit PlayerGameObj():
		IGameObject("Player"), playerlight(nullptr), camera(nullptr)
	{
	}
	// virtual (override of IGameObject)
	// called by bullet
	virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
	{
		IGameObject::setWorldTransform(centerOfMassWorldTrans);
		btVector3 pos = m_graphicsWorldTrans.getOrigin();
		camera->setPosition(core::vector3df(pos.x(), pos.y(), pos.z()));
	}

	virtual void render(GameLoop&, float);
	virtual void initialize(GameLoop&);
	virtual void logic_tick(GameLoop&);

	virtual ~PlayerGameObj() {};
};


class StaticStructure: public IGameObject {
	scene::IAnimatedMeshSceneNode* map_node;
	W_Structure structure_data;
	// bullet fields are not remembered: the position is constant...

	virtual void render(GameLoop&, float);
	virtual void initialize(GameLoop&, const W_Structure& structure_data);
	virtual void logic_tick(GameLoop&);

	virtual ~StaticStructure() {};
};
#endif
