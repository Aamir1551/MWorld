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

    set<Block *> blocks_at_leaf;
    set<IBlock *> iblocks_at_leaf;
    set<ZBlock *> zblocks_at_leaf;
    set<EBlock *> eblocks_at_leaf;
    set<MBlock *> mblocks_at_leaf;

    std::map<Octree *, vector<Octree *> > grid_elements_neighbours;

    real avg_x, avg_y, avg_z, min_x, max_x, min_y, max_y, min_z, max_z;
    real sum_x_i, sum_y_i, sum_z_i, sum_x_z, sum_y_z, sum_z_z, sum_x_m, sum_y_m, sum_z_m, sum_x_e, sum_y_e, sum_z_e;


    bool is_leaf = false;
    std::map<unsigned int, Octree*> children;


    explicit Octree(int grid_sizes, real min_x, real  max_x, real min_y, real max_y, real min_z, real max_z, bool initialise = false);
    Octree* AddBlock(Block *b);

    Octree* AddIBlock(IBlock *b);
    Octree* AddZBlock(ZBlock *b);
    Octree* AddEBlock(EBlock *b);
    Octree* AddMBlock(MBlock *b);

    void RemoveIBlock(IBlock *b);
    void RemoveZBlock(ZBlock *b);
    void RemoveEBlock(EBlock *b);
    void RemoveMBlock(MBlock *b);


    bool static BlockInCorrectTree(Octree * tree, Block *b);

    void RemoveBlock(Block *b);
    std::vector<Octree *> GetGridNeighbours(real x, real y, real z);
    void AddGridAtPosToVec(real x, real y, real z, vector<Octree *> &octree_list);
    Octree* GetGridAtPos(real x, real y, real z);

    bool LeafsAreNull();

};
#endif