
#pragma once

#include <irrList.h>
#include <vector3d.h>


#include "W_Structure.h"

using namespace irr;

class W_LevelGenerator {
 public:
  W_LevelGenerator(core::vector3d<int>);

  core::list<W_Structure> * getStructures();
  core::vector3d<int> getSize();

 private:

 core::list<W_Structure> structures;
 core::vector3d<int> size;

};
