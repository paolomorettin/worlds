
#include <stdlib.h>
#include "W_LevelGenerator.h"


W_LevelGenerator::W_LevelGenerator(core::vector3d<int> size ){
  this -> size = size;

  core::vector3d<int> maxsize = core::vector3d<int>(size.X, size.Y, size.Z);

  bool top_reached = false;
  int skyline[size.X][size.Z];

  // init skyline matrix
  for (int x = 0; x < size.X; x++)
    for (int z = 0; z < size.Z; z++)
      skyline[x][z] = 0;
    
  int struct_count = 0;
  while (struct_count < 10){
    struct_count += 1;
    // randomly generate the structure parameters(size,position,etc)
    int pos_x = rand() % size.X;
    int pos_z = rand() % size.Z;
    int size_x = 1 + rand() % (maxsize.X - pos_x);
    int size_z = 1 + rand() % (maxsize.Z - pos_z);

    // pos_y depends on the current skyline
    int highest = 0;
    for (int i=pos_x; i < pos_x + size_x; i++)
      for (int j=pos_z; j < pos_z + size_z; j++){
	if (skyline[i][j] > highest)
	  highest = skyline[i][j];
      }

    // we reached the top, stop adding elements to the level
    if (highest == size.Y)
      break;

    int pos_y = highest;
    int size_y = 1 + rand() % (maxsize.Y - pos_y);

    
    (this -> structures).push_front( W_Structure(pos_x,pos_y,pos_z,size_x,size_y,size_z) );
    
    
  }
}



// getters
core::list<W_Structure> * W_LevelGenerator::getStructures(){ return &(this -> structures); }
core::vector3d<int> W_LevelGenerator::getSize(){ return this -> size; }
