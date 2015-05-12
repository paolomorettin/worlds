

#pragma once
#include <irrlicht.h>


class W_Timer {
 public :
  W_Timer(irr::IrrlichtDevice*);
  void start();
  void pause();
  void resume();
  int getTime();
  bool isPaused();
  
 private :
  bool paused;
  irr::IrrlichtDevice* device;
  int span;

};
