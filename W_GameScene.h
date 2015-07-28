#ifndef W_GAME_SCENE_H
#define W_GAME_SCENE_H

#include "W_GameLoop.h"
#include "W_GameObject.h"

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
	btVector3 position; // of the bounding sphere
	scene::ICameraSceneNode* camera; // irr camera
	scene::ILightSceneNode* playerlight; // irr light that follows the player.

public:

	explicit PlayerGameObj():
		IGameObject("Player"), position(0,0,0), playerlight(nullptr), camera(nullptr)
	{
		
	}
	// virtual (override of IGameObject)
	// called by bullet
	virtual void setWorldTransform(const btTransform& centerOfMassWorldTrans)
	{
		m_graphicsWorldTrans = centerOfMassWorldTrans * m_centerOfMassOffset;
		btVector3 pos = m_graphicsWorldTrans.getOrigin();
		camera->setPosition(core::vector3df(pos.x(), pos.y(), pos.z()));
	}

	virtual void render(GameLoop&, float);
	virtual void initialize(GameLoop&);
	virtual void logic_tick(GameLoop&);

	virtual ~PlayerGameObj() {};
};

class StaticCube: public IGameObject {

	virtual void render(GameLoop&, float);
	virtual void logic_tick(GameLoop&);

	virtual ~StaticCube() {};
};
#endif
