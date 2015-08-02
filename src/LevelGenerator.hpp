
#pragma once

#include <irrList.h>
#include <vector3d.h>

#include "Block.hpp"

using namespace irr;
using namespace core;

class LevelGenerator {
 public:
    LevelGenerator(vector3d<int>,int);

    list<Block>* getStructures();
    vector3d<int> getSize();
    vector3d<int> getStart();
    vector3d<int> getEnd();

 private:
    list<Block> structures;
    vector3d<int> size;
    vector3d<int> start;
    vector3d<int> end;

};
