#pragma once

class GameLoop;

class IScene {
 public:
    virtual ~IScene() {};
    virtual bool create_scene(GameLoop& loop) = 0;
};
