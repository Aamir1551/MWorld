#ifndef MWORLD_SIMULATION_OCTREE
#define MWORLD_SIMULATION_OCTREE

#include <map>
#include <vector>

#include <block.hpp>
#include <settings.hpp>

using namespace settings;
using namespace blocks;


class Octree {
public:

    real partition_size;
    int grid_size;

    std::map<unsigned int, Block *> world_blocks;
    Block*block = nullptr;
    std::map<Octree *, vector<Octree *> > grid_elements_neighbours;

    real avg_x;
    real avg_y;
    real avg_z;
    real min_x;
    real max_x;
    real min_y;
    real max_y;
    real min_z;
    real max_z;

    bool is_leaf = false;
    std::map<unsigned int, Octree*> children;


    explicit Octree(int grid_sizes, real min_x, real  max_x, real min_y, real max_y, real min_z, real max_z, bool initialise = false);
    void AddBlock(Block *b);
    void RemoveBlock(Block *b);
    std::vector<Octree *> GetGridNeighbours(real x, real y, real z);
    void AddGridAtPosToVec(real x, real y, real z, vector<Octree *> &octree_list);
    Octree* GetGridAtPos(real x, real y, real z);

};
#endif