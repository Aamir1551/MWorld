#ifndef MWORLD_SIMULATION_OCTREE
#define MWORLD_SIMULATION_OCTREE

#include <map>
#include <set>
#include <vector>
#include <tuple>


#include <matrix.hpp>

#include <block.hpp>
#include <i_block.hpp>
#include <m_block.hpp>
#include <z_block.hpp>
#include <e_block.hpp>

#include <settings.hpp>

using namespace settings;

namespace blocks {
    class Octree {
    public:

        real partition_size;
        int grid_size;

        set<Block *> blocks_at_leaf;

        set<IBlock *> iblocks_at_leaf_neg;
        set<IBlock *> iblocks_at_leaf_plus;

        set<ZBlock *> zblocks_at_leaf;
        set<EBlock *> eblocks_at_leaf;
        set<MBlock *> mblocks_at_leaf_plus;
        set<MBlock *> mblocks_at_leaf_neg;

        std::map<Octree *, vector<Octree *> > grid_elements_neighbours;

        real avg_x, avg_y, avg_z, min_x, max_x, min_y, max_y, min_z, max_z;
        real sum_x_i_plus, sum_y_i_plus, sum_z_i_plus,
        sum_x_i_neg, sum_y_i_neg, sum_z_i_neg,
        sum_x_z, sum_y_z, sum_z_z,
        sum_x_m_plus, sum_y_m_plus, sum_z_m_plus,
        sum_x_m_neg, sum_y_m_neg, sum_z_m_neg,
        sum_x_e, sum_y_e, sum_z_e;


        Matrix com_i_plus, com_i_neg, com_m_plus, com_m_neg, com_e, com_z;

        bool is_leaf = false;
        std::map<unsigned int, Octree *> children;


        explicit Octree(int grid_sizes, real min_x, real max_x, real min_y, real max_y, real min_z, real max_z,
                        bool initialise = false);

        //Octree* AddBlock(Block *b);

        void CalculateCOMS();

        Octree *AddIBlockPlus(IBlock *b);
        Octree *AddIBlockNeg(IBlock *b);

        Octree *AddZBlock(ZBlock *b);

        Octree *AddEBlock(EBlock *b);

        Octree *AddMBlockPlus(MBlock *b);
        Octree *AddMBlockNeg(MBlock *b);

        //void RemoveBlock(Block *b);

        void RemoveIBlockPlus(IBlock *b);
        void RemoveIBlockNeg(IBlock *b);

        void RemoveZBlock(ZBlock *b);

        void RemoveEBlock(EBlock *b);

        void RemoveMBlockPlus(MBlock *b);
        void RemoveMBlockNeg(MBlock *b);


        bool static BlockInCorrectTree(Octree *tree, Block *b);

        std::vector<Octree *> GetGridNeighbours(real x, real y, real z);

        void AddGridAtPosToVec(real x, real y, real z, vector<Octree *> &octree_list);

        Octree *GetGridAtPos(real x, real y, real z);

        bool LeafsAreNull();

    };
}
#endif