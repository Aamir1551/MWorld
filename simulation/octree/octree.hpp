#ifndef MWORLD_SIMULATION_OCTREE
#define MWORLD_SIMULATION_OCTREE

#include <map>
#include <vector>
#include <tuple>

#include <block.hpp>
#include <i_block.hpp>
#include <m_block.hpp>
#include <z_block.hpp>
#include <e_block.hpp>

#include <settings.hpp>

using namespace settings;
using namespace blocks;


class Octree {
public:

    real partition_size;
    int grid_size;

    map<int, Block *> blocks_at_leaf;
    map<int, IBlock *> iblocks_at_leaf;
    map<int, ZBlock *> zblocks_at_leaf;
    map<int, EBlock *> eblocks_at_leaf;
    map<int, MBlock *> mblocks_at_leaf;

    std::map<Octree *, vector<Octree *> > grid_elements_neighbours;

    real avg_x, avg_y, avg_z, min_x, max_x, min_y, max_y, min_z, max_z;
    real com_x_i, com_y_i, com_z_i, com_x_z, com_y_z, com_z_z, com_x_m, com_y_m, com_z_m, com_x_e, com_y_e, com_z_e;

    bool is_leaf = false;
    std::map<unsigned int, Octree*> children;


    explicit Octree(int grid_sizes, real min_x, real  max_x, real min_y, real max_y, real min_z, real max_z, bool initialise = false);
    Octree* AddBlock(Block *b, int id);

    Octree* AddIBlock(IBlock *b, int id);
    Octree* AddZBlock(ZBlock *b, int id);
    Octree* AddEBlock(EBlock *b, int id);
    Octree* AddMBlock(MBlock *b, int id);

    void RemoveIBlock(IBlock *b, int id);
    void RemoveZBlock(ZBlock *b, int id);
    void RemoveEBlock(EBlock *b, int id);
    void RemoveMBlock(MBlock *b, int id);


    bool static BlockInCorrectTree(Octree * tree, Block *b);

    void RemoveBlock(Block *b, int id);
    std::vector<Octree *> GetGridNeighbours(real x, real y, real z);
    void AddGridAtPosToVec(real x, real y, real z, vector<Octree *> &octree_list);
    Octree* GetGridAtPos(real x, real y, real z);

    bool LeafsAreNull();

};
#endif