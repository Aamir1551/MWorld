//
// Created by aamir on 06/04/2021.
//

#ifndef MWORLD_OCTREE_OCTREE_H
#define MWORLD_OCTREE_OCTREE_H

#include <block.hpp>

#include <settings.hpp>

using namespace settings;

namespace blocks {
    class Octree {

    public:

        int grid_size;

        real min_x = 0, max_x = 0, min_y = 0, max_y = 0, min_z = 0, max_z = 0;
        real avg_x = 0, avg_y = 0, avg_z = 0;
        real cell_partition_size;

        Octree(int grid_size, real min_x, real max_x, real min_y, real max_y, real min_z, real max_z);

        static bool BlockInCorrectTree(Octree *tree, Block *b);

        bool is_leaf = false;
    };
}


#endif