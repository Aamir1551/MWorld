//
// Created by aamir on 06/04/2021.
//

#include <cmath>
#include <octree.hpp>

#include <block.hpp>

#include <settings.hpp>

using namespace settings;
using namespace blocks;


bool Octree::BlockInCorrectTree(Octree *tree, Block *b) {

    real x = b->position(0,0);
    real y = b->position(1,0);
    real z = b->position(2,0);
    if(tree->min_x <= x && tree->max_x >= x && tree->min_y <= y && tree->max_y >= y && tree->min_z <= z && tree->max_z >= z) {
        return true;
    }
    return false;

}

Octree::Octree(int grid_size, real min_x, real  max_x, real min_y, real max_y, real min_z, real max_z) : grid_size(grid_size), min_x(min_x), max_x(max_x), min_y(min_y), max_y(max_y), min_z(min_z), max_z(max_z){

    this->cell_partition_size = std::min(max_x - min_x, std::min(max_y - min_y, max_z - min_z));
    avg_x = (min_x + max_x) / 2;
    avg_y = (min_y + max_y) / 2;
    avg_z = (min_z + max_z) / 2;
};
