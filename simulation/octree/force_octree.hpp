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
    class ForceOctree {
    public:

        real cell_partition_size;
        int grid_size;

        set<Block *> blocks_at_leaf;

        set<IBlock *> iblocks_at_leaf_neg;
        set<IBlock *> iblocks_at_leaf_plus;

        set<ZBlock *> zblocks_at_leaf;
        set<EBlock *> eblocks_at_leaf;
        set<MBlock *> mblocks_at_leaf_plus;
        set<MBlock *> mblocks_at_leaf_neg;

        std::map<ForceOctree *, vector<ForceOctree *> > grid_elements_neighbours;

        real avg_x, avg_y, avg_z, min_x, max_x, min_y, max_y, min_z, max_z;
        Matrix sum_i_plus,  sum_i_neg, sum_z, sum_m_plus, sum_m_neg, sum_e;



        Matrix com_i_plus = Matrix(3, 1);
        Matrix com_i_neg = Matrix(3, 1);
        Matrix com_m_plus = Matrix(3, 1);
        Matrix com_m_neg = Matrix(3, 1);
        Matrix com_e = Matrix(3, 1);
        Matrix com_z = Matrix(3, 1);

        bool is_leaf = false;
        std::map<unsigned int, ForceOctree *> children;


        explicit ForceOctree(int grid_sizes, real min_x, real max_x, real min_y, real max_y, real min_z, real max_z,
                        bool initialise = false);

        void CalculateCOMS();

        ForceOctree *AddIBlockPlus(IBlock *b);
        ForceOctree *AddIBlockNeg(IBlock *b);
        ForceOctree *AddZBlock(ZBlock *b);
        ForceOctree *AddEBlock(EBlock *b);
        ForceOctree *AddMBlockPlus(MBlock *b);
        ForceOctree *AddMBlockNeg(MBlock *b);

        void RemoveIBlockPlus(IBlock *b);
        void RemoveIBlockNeg(IBlock *b);
        void RemoveZBlock(ZBlock *b);
        void RemoveEBlock(EBlock *b);
        void RemoveMBlockPlus(MBlock *b);
        void RemoveMBlockNeg(MBlock *b);

        static int count;

        int iblocks_at_cell_plus_count, iblocks_at_cell_neg_count, mblocks_at_cell_plus_count, mblocks_at_cell_neg_count, zblocks_at_cell_count, eblocks_at_cell_count;

        void ApplyBarnesHutOnBlock(Block *b, real delta_time);

        void CalculateCOMonTree();

        bool static BlockInCorrectTree(ForceOctree *tree, Block *b);

        bool LeafsAreNull();

    };
}
#endif