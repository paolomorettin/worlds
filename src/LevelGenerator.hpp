
#pragma once

#include <irrList.h>
#include <vector3d.h>


#include "Structure.hpp"

using namespace irr;
using namespace core;

class LevelGenerator {
 public:
    LevelGenerator(vector3d<int>,int);

    list<Structure>* getStructures();
    vector3d<int> getSize();
    vector3d<int> getStart();
    vector3d<int> getEnd();

 private:
    list<Structure> structures;
    vector3d<int> size;
    vector3d<int> start;
    vector3d<int> end;

};
