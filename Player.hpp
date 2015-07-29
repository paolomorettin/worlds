#pragma once

#include "GameLoop.hpp"
#include "GameObject.hpp"
#include "LevelGenerator.hpp"

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

