#pragma once

#include "IScene.hpp"
#include "IGameObject.hpp"
#include "Block.hpp"
#include <irrlicht.h>

class GameLoop;
class Player;


class GameScene : public IScene {
    const float world_scale = 1; // affects structures
    Player* playerObj = nullptr;

 public:
    virtual bool create_scene(GameLoop& loop);
};


class btRigidBody;

class StaticStructure: public IGameObject {
    irr::scene::IAnimatedMeshSceneNode* map_node;
    Block structure_data;
    // bullet fields are not remembered: the position is constant...
public:
    explicit StaticStructure(): structure_data(0,0,0,0,0,0), map_node(nullptr) {};
    virtual void render(GameLoop&, float);
    virtual btRigidBody* initialize(GameLoop&, const Block& structure_data);
    virtual void logic_tick(GameLoop&);
    virtual void notify(const irr::SEvent& evt);

    virtual ~StaticStructure() {};
};
