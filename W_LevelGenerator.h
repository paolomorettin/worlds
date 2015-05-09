
#pragma once

#include <irrList.h>
#include <vector3d.h>


#include "W_Structure.h"

using namespace irr;
using namespace core;

class W_LevelGenerator {
 public:
  W_LevelGenerator(vector3d<int>,int);

  list<W_Structure> * getStructures();
  vector3d<int> getSize();
  vector3d<int> getStart();
  vector3d<int> getEnd();

 private:
  list<W_Structure> structures;
  vector3d<int> size;
  vector3d<int> start;
  vector3d<int> end;

};
