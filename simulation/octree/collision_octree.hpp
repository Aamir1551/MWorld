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

        // Stores the blocks at the leaf
        set<Block *> blocks_at_leaf;

        // cache that stores the neighbours of the node. Note: This is empty for all nodes, except in the case of node is a leaf
        std::map<CollisionOctree *, vector<CollisionOctree *> > grid_elements_neighbours;

        // Stores the children of the current node
        CollisionOctree* children[8];

        explicit CollisionOctree(int grid_sizes, real min_x, real max_x, real min_y, real max_y, real min_z, real max_z,
                                 bool initialise = false);

        // Add a block to the octree. Traverses the collision_tree using the position of the block to find the leaf node to store the block.
        CollisionOctree *AddBlock(Block *b);

        // Removes a block from the octree. Traverses the collision_tree using the position of the block to find the leaf node storing the block.
        void RemoveBlock(Block *b);

        // Returns the leaf node at the coordinates given
        CollisionOctree *GetGridAtPos(real x, real y, real z);

        ~CollisionOctree();

private:
    // Function to generate the neighbours of each leaf node
    std::vector<CollisionOctree *> GenerateNeigbours(real x, real y, real z, real partition_min_x, real partition_min_y, real partition_min_z);

    // Function used for testing purposes, to test if the collision_tree does not contain any blocks at any cells in the octree
    bool LeavesAreNull();

    // Auxiliary function used to set the neighbours of the octree.
    void AddGridAtPosToVec(real x, real y, real z, vector<CollisionOctree *> &octree_list);

};
}
#endif