
#include <stdlib.h>
#include "LevelGenerator.hpp"




LevelGenerator::LevelGenerator(vector3d<int> lvl_size, int elements ){
    this -> size = lvl_size;

    //core::vector3d<int> maxsize = core::vector3d<int>(size.X, size.Y, size.Z);
    vector3d<int> minsize = vector3d<int>(1, 1, 1);
    vector3d<int> maxsize = vector3d<int>(3, 3, 3);

    int skyline[lvl_size.X][lvl_size.Z];
    bool bitmap[lvl_size.X][lvl_size.Y][lvl_size.Z];

    // init skyline matrix
    for (int x = 0; x < lvl_size.X; x++) {
        for (int z = 0; z < lvl_size.Z; z++){
            skyline[x][z] = 0;
            for (int y = 0; y < lvl_size.Y; y++) {
                bitmap[x][y][z] = true;
            }
        }
    }

    int struct_count = 0;
    while (struct_count < elements){
        struct_count += 1;
        // randomly generate the structure parameters(size,position,etc)
        int size_x = minsize.X + (rand() % (maxsize.X - minsize.X + 1));
        int size_z = minsize.Z + (rand() % (maxsize.Z - minsize.Z + 1));

        int pos_x = rand() % (lvl_size.X - size_x);
        int pos_z = rand() % (lvl_size.Z - size_z);


        // pos_y depends on the current skyline
        int highest = 0;
        for (int i=pos_x; i < pos_x + size_x; i++) {
            for (int j=pos_z; j < pos_z + size_z; j++) {
                if (skyline[i][j] > highest) {
                    highest = skyline[i][j];
                }
            }
        }


        int pos_y = highest;
        int size_y = minsize.Y + (rand() % (maxsize.Y - minsize.Y + 1));

        // if a newly generated structure reached the top, stop adding elements to the level
        // not sure if this is a good thing
        if ((pos_y + size_y) >= size.Y)
            break;

        printf("Position: (%i,%i,%i) Size: (%i,%i,%i)\n",pos_x,pos_y,pos_z,size_x,size_y,size_z);
        (this -> structures).push_front( Block(pos_x,pos_y,pos_z,size_x,size_y,size_z) );

        // update the skyline and the bitmap
        for (int i=pos_x; i < pos_x + size_x; i++) {
            for (int j=pos_z; j < pos_z + size_z; j++) {
                skyline[i][j] = pos_y + size_y;
                for (int k=pos_y; k < pos_y + size_y; k++) {
                    bitmap[i][k][j] = false;
                }
            }
        }
    }

    // generate start and end points
    // greedy search maximizing manhattan distance
    vector3d<int> p1 = vector3d<int>(0,1,0);
    vector3d<int> p2 = vector3d<int>(lvl_size.X - 1, lvl_size.Y - 1, lvl_size.Z - 1);

    int max_distance = 0;

    const int max_iterations = 10;
    for (int i = 0; i < max_iterations; i++) {

        while (!bitmap[p1.X][p1.Y][p1.Z] || !bitmap[p2.X][p2.Y][p2.Z] || p1 == p2 || bitmap[p1.X][p1.Y - 1][p1.Z] || bitmap[p2.X][p2.Y - 1][p2.Z]) {
            p1.X = rand() % lvl_size.X;
            p1.Y = 1 + rand() % (lvl_size.Y - 1);
            p1.Z = rand() % lvl_size.Z;
            p2.X = rand() % lvl_size.X;
            p2.Y = rand() % lvl_size.Y;
            p2.Z = rand() % lvl_size.Z;
        }

        int distance = abs(p1.X - p2.X) + abs(p1.Y - p2.Y) + abs(p1.Z - p2.Z);
        if (distance > max_distance) {
            max_distance = distance;
            this -> start = p1;
            this -> end = p2;
        }
    }
}


// getters
list<Block> * LevelGenerator::getStructures(){ return &(this -> structures); }
vector3d<int> LevelGenerator::getSize(){ return this -> size; }
vector3d<int> LevelGenerator::getStart(){ return this -> start; }
vector3d<int> LevelGenerator::getEnd(){ return this -> end; }
