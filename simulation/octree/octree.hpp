#ifndef MWORLD_SIMULATION_OCTREE
#define MWORLD_SIMULATION_OCTREE

#include <vector>
#include <map>

#include <block.hpp>
#include <settings.hpp>

using namespace settings;
using namespace blocks;


class Octree {
public:

    real partition_size;
    real world_size;
    std::map<int, Block *> world_blocks;
    real avg_x;
    real avg_y;
    real avg_z;
    bool is_min = false;
    std::map<int, Octree> children;


    Octree(real grid_sizes, real min_x, real  max_x, real min_y, real max_y, real min_z, real max_z);

    void AddBlock(Block *b, unsigned int id);
    void RemoveBlock(Block *b, unsigned int id);

};
#endif