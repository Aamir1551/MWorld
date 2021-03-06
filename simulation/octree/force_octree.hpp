#ifndef MWORLD_SIMULATION_FORCE_OCTREE
#define MWORLD_SIMULATION_FORCE_OCTREE

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

        int grid_size;

        std::map<ForceOctree *, vector<ForceOctree *> > grid_elements_neighbours;

        real min_x = 0, max_x = 0, min_y = 0, max_y = 0, min_z = 0, max_z = 0;
        real avg_x = 0, avg_y = 0, avg_z = 0;
        Matrix sum_i_plus,  sum_i_neg, sum_z, sum_m_plus, sum_m_neg, sum_e;

        Matrix com_i_plus = Matrix(3, 1);
        Matrix com_i_neg = Matrix(3, 1);
        Matrix com_m_plus = Matrix(3, 1);
        Matrix com_m_neg = Matrix(3, 1);
        Matrix com_e = Matrix(3, 1);
        Matrix com_z = Matrix(3, 1);

        real cell_partition_size;

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

        void RemoveAllZBlocks();

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

    };
}
#endif