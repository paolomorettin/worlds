
#include "W_Timer.h"

W_Timer::W_Timer(irr::IrrlichtDevice* device){
  this -> device = device;
  this -> paused = true;
  this -> span = 0;
}



void W_Timer::start(){
  paused = false;
  span = device -> getTimer() -> getTime();
}
void W_Timer::pause(){
  paused = true;
  span = device -> getTimer() -> getTime() - span;
}
void W_Timer::resume(){
  paused = false;
  span = device -> getTimer() -> getTime() - span;

}
int W_Timer::getTime(){
  if (paused)
    return span;
  else
    return device -> getTimer() -> getTime() - span;
}
  
bool W_Timer::isPaused(){
  return paused;
}
