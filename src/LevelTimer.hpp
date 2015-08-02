#pragma once

#include "GameLoop.hpp"
#include "IGameObject.hpp"
#include "LevelGenerator.hpp"


class LevelTimer : public IGameObject {
    long current_time_ticks = 0; //in ticks
    bool started = false;

    gui::IGUIStaticText* timer_text = nullptr;
    gui::IGUIFont* font = nullptr;
public:

    void initialize(GameLoop&);

    inline void start() { started = true; }
    inline void stop() { started = false; }
    inline void reset() { this->set(0); }
    inline void set(int time) { current_time_ticks = time; update_text(); }
    inline long get() const { return current_time_ticks; }

    virtual void logic_tick(GameLoop&) {
        if (started) {
            current_time_ticks++;
            update_text();
        }
    }

private:
    void update_text();
};

