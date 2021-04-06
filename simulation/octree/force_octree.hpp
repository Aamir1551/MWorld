#ifndef MWORLD_SIMULATION_OCTREE_FORCE_OCTREE
#define MWORLD_SIMULATION_OCTREE_FORCE_OCTREE

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
    class ForceOctree : public Octree {
    public:


        // Storing the Center of Mass of each block that lives in the current cell
        Matrix sum_i_plus,  sum_i_neg, sum_z, sum_m_plus, sum_m_neg, sum_e;

        // Center of mass of each block type at the current cell
        Matrix com_i_plus = Matrix(3, 1);
        Matrix com_i_neg = Matrix(3, 1);
        Matrix com_m_plus = Matrix(3, 1);
        Matrix com_m_neg = Matrix(3, 1);
        Matrix com_e = Matrix(3, 1);
        Matrix com_z = Matrix(3, 1);

        // Children of the ForceOctree. The 3D cell this octree represent is also split into 8 equal regions
        ForceOctree* children[8];


        explicit ForceOctree(int grid_sizes, real min_x, real max_x, real min_y, real max_y, real min_z, real max_z);

        // Sets the com_* variables according to the objects currently living at the region represented by the octree
        void CalculateCOMS();

        // Functions to add the different blocks to the octree
        ForceOctree *AddIBlockPlus(IBlock *b);
        ForceOctree *AddIBlockNeg(IBlock *b);
        ForceOctree *AddZBlock(ZBlock *b);
        ForceOctree *AddEBlock(EBlock *b);
        ForceOctree *AddMBlockPlus(MBlock *b);
        ForceOctree *AddMBlockNeg(MBlock *b);

        // Functions to Remove the different blocks from the octree
        void RemoveIBlockPlus(IBlock *b);
        void RemoveIBlockNeg(IBlock *b);
        void RemoveZBlock(ZBlock *b);
        void RemoveEBlock(EBlock *b);
        void RemoveMBlockPlus(MBlock *b);
        void RemoveMBlockNeg(MBlock *b);

        static int count;

        // Keep a count of the different blocks stored at region represented by current octree
        int iblocks_at_cell_plus_count, iblocks_at_cell_neg_count, mblocks_at_cell_plus_count, mblocks_at_cell_neg_count, zblocks_at_cell_count, eblocks_at_cell_count;

        // Apply Barnes-Hut algorithm
        void ApplyBarnesHutOnBlock(Block *b, real delta_time);

        // Traverse the entire tree from this node, and calculate the COM of each node
        void CalculateCOMonTree();

        ~ForceOctree();

    };
}
#endif