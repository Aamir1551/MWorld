#ifndef MWORLD_SIMULATION_OCTREE
#define MWORLD_SIMULATION_OCTREE

#include <vector>

#include <block.hpp>
#include <settings.hpp>

using namespace settings;
using namespace blocks;


class Octree {
public:

    real partition_size;
    real world_size;
    vector<Block*> world_blocks;


    Octree(real partition_sizes, real world_size);

    void AddBlock(Block *b);
};
#endif