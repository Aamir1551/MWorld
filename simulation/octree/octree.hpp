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

        int grid_size;

        set<Block *> blocks_at_leaf;
        std::map<Octree *, vector<Octree *> > grid_elements_neighbours;

        real min_x = 0, max_x = 0, min_y = 0, max_y = 0, min_z = 0, max_z = 0;
        real avg_x = 0, avg_y = 0, avg_z = 0;
        real cell_partition_size;

        bool is_leaf = false;
        std::map<unsigned int, Octree *> children;

        explicit Octree(int grid_sizes, real min_x, real max_x, real min_y, real max_y, real min_z, real max_z,
                        bool initialise = false);

        Octree *AddBlock(Block *b);

        void RemoveBlock(Block *b);

        static int count;

        bool static BlockInCorrectTree(Octree *tree, Block *b);

        std::vector<Octree *> GenerateNeigbours(real x, real y, real z);

        void AddGridAtPosToVec(real x, real y, real z, vector<Octree *> &octree_list);

        Octree *GetGridAtPos(real x, real y, real z);

        bool LeafsAreNull();

    };
}
#endif