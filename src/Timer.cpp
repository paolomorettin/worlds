
#include "Timer.hpp"

Timer::Timer(irr::IrrlichtDevice* device){
  this -> device = device;
  this -> paused = true;
  this -> span = 0;
}



void Timer::start(){
  paused = false;
  span = device -> getTimer() -> getTime();
}
void Timer::pause(){
  if (!paused){
    paused = true;
    span = device -> getTimer() -> getTime() - span;
  }
}
void Timer::resume(){
  if (paused){
    paused = false;
    span = device -> getTimer() -> getTime() - span;
  }

}
int Timer::getTime(){
  if (paused)
    return span;
  else
    return device -> getTimer() -> getTime() - span;
}
  
bool Timer::isPaused(){
  return paused;
}
