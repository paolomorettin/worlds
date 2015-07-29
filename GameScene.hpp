#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "GameLoop.hpp"
#include "GameObject.hpp"
#include "LevelGenerator.hpp"
#include "Player.hpp"


class MainGameScene : public IGameObject {
 public:

	const float world_scale = 1; // affects structures
	
	PlayerGameObj* playerObj = nullptr;

	virtual void render(GameLoop&, float);
	virtual void logic_tick(GameLoop&);

    bool initialize(GameLoop& loop);
	
	virtual ~MainGameScene() {};
};


class btRigidBody;

class StaticStructure: public IGameObject {
	scene::IAnimatedMeshSceneNode* map_node;
	Structure structure_data;
	// bullet fields are not remembered: the position is constant...
 public:
	explicit StaticStructure(): structure_data(0,0,0,0,0,0), map_node(nullptr) {};
	virtual void render(GameLoop&, float);
	virtual btRigidBody* initialize(GameLoop&, const Structure& structure_data);
	virtual void logic_tick(GameLoop&);

	virtual ~StaticStructure() {};
};
#endif
