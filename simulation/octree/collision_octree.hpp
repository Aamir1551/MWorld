#ifndef MWORLD_SIMULATION_OCTREE_COLLISION_OCTREE
#define MWORLD_SIMULATION_OCTREE_COLLISION_OCTREE

#include <map>
#include <set>
#include <vector>
#include <tuple>

#include <octree.hpp>

#include <matrix.hpp>

#include <block.hpp>
#include <i_block.hpp>
#include <m_block.hpp>
#include <z_block.hpp>
#include <e_block.hpp>

#include <settings.hpp>

using namespace settings;

namespace blocks {
class CollisionOctree : public Octree {
    public:

        set<Block *> blocks_at_leaf;
        std::map<CollisionOctree *, vector<CollisionOctree *> > grid_elements_neighbours;

        CollisionOctree* children[8];

        explicit CollisionOctree(int grid_sizes, real min_x, real max_x, real min_y, real max_y, real min_z, real max_z,
                                 bool initialise = false);

        CollisionOctree *AddBlock(Block *b);

        void RemoveBlock(Block *b);

        static int count;

        void AddGridAtPosToVec(real x, real y, real z, vector<CollisionOctree *> &octree_list);

        CollisionOctree *GetGridAtPos(real x, real y, real z);

        bool LeafsAreNull();
        ~CollisionOctree();

private:
    std::vector<CollisionOctree *> GenerateNeigbours(real x, real y, real z, real partition_min_x, real partition_min_y, real partition_min_z);

};
}
#endif