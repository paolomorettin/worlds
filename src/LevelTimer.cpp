#include "LevelTimer.hpp"

void LevelTimer::initialize(GameLoop& loop) {
    timer_text = loop.guienv -> addStaticText(L"0.0000", rect<s32>(10,10,260,50),false,false,0);
    font = loop.guienv->getFont("./media/bigfont.png"); 
    timer_text -> setOverrideColor(video::SColor(255,255,255,255));
    if (font){
      timer_text -> setOverrideFont(font);
    }

    core::stringw str = L"";
    str += 0.0f;
    timer_text->setText(str.c_str());
    //int snprintf ( char * s, size_t n, const char * format, ... );
    loop.attach(this);
}



void LevelTimer::update_text() {
    core::stringw str = L"";     
    str += ((float)current_time_ticks)/300.0f;
    timer_text->setText(str.c_str());
}
