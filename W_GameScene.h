#ifndef W_GAME_SCENE_H
#define W_GAME_SCENE_H

#include "W_GameLoop.h"
#include "W_GameObject.h"

class MainGameScene : public IGameObject {
 public:

	scene::ICameraSceneNode* camera;

	virtual void render(GameLoop&, float);
	virtual void logic_tick(GameLoop&);

    bool initialize(GameLoop& loop);
	
	virtual ~MainGameScene() {};
};

#endif
