#ifndef MWORLD_SIMULATION_WORLD_HANDLER_H
#define MWORLD_SIMULATION_WORLD_HANDLER_H

#include <cstdlib>
#include <ctime>

#include <vector>
#include <tuple>
#include <set>
#include <unordered_set>
#include <utility>
#include <omp.h>

#include <matrix.hpp>

#include <cube.hpp>
#include <block.hpp>
#include <i_block.hpp>
#include <m_block.hpp>
#include <e_block.hpp>
#include <z_block.hpp>

#include <collision_octree.hpp>
#include <force_octree.hpp>

using namespace blocks;

class WorldHandler {

private:

    /**
     * @brief Generates positions and linear momentum vectors. Amount generated is num_blocks.
     *
     * @param num_blocks Number of positions and linear momentum's to generate
     * @param positions Vector that is filled with the initial positions of the blocks.
     * @param linear_momentum's vector that is filled with the initial linear momentum's of the blocks
     * @param min_coord_x, max_coord_x,  min_coord_y, max_coord_y,  min_coord_z, max_coord_z represent the world dimensions
     */
    void static GetProperties(int num_blocks, std::vector<Matrix> *&positions, std::vector<Matrix> * &linear_momentums, real min_coord_x, real max_coord_x,  real min_coord_y, real max_coord_y,  real min_coord_z, real max_coord_z);

    // Below functions generate positions and linear momentum's
    std::vector<Matrix> static *GeneratePositions(int num_cubes, real min_coord_x, real max_coord_x, real min_coord_y, real max_coord_y, real min_coord_z, real max_coord_z);
    std::vector<Matrix> static *GenerateLinearMomentums(int num_cubes);


    /**
     * @brief passes flare from each block to the other. Flare can only be passed if blocks are in contact with each
     * other, hence we take in the contact list, and then pass flare from one block to another.
     *
     * @param contacts Represents a list of contacts between blocks in the world
     * @param delta_time Duration over which the contacts took place
     * */
    void static PassBlockFlares(vector<Contact> &contacts, real delta_time);

    // Passes flare from one block to another, over the time frame delta_time given
    void static PassFlare(Block *a, Block *b, real delta_time);


#if defined(OPENMP)
    // Returns a set of all the leaves that are currently preoccupied by the blocks in the world. This function
    // calculates this in parallel using multiple threads.
    std::unordered_set<CollisionOctree*> GetBlockPositionsParallel();
#endif

    // Returns a set of all the leaves that are currently preoccupied by the blocks in the world. This function
    // calculates this using a single thread.
    std::unordered_set<CollisionOctree*> GetBlockPositionsSerial();

    // Creates a DAG using the active leaves as nodes in the DAG, and joining them with directed edges, in such a way
    // to ensure that no cycle is formed
    void MakeDAG(std::unordered_set<CollisionOctree *> &block_positions, std::vector<pair<CollisionOctree *, CollisionOctree *>> &edges, std::unordered_set<CollisionOctree *> &nodes);

    // Create a DAG using the "root" as the root of the DAG
    void GetDAGAtRoot(CollisionOctree *root, std::vector<pair<CollisionOctree *, CollisionOctree *>> &edges, std::unordered_set<CollisionOctree *> &nodes);

    // Octrees for collision handling and for applying Barnes-Hut algorithm
    CollisionOctree *collision_tree;
    ForceOctree *forces_tree;

    // Stores all the blocks in the world
    vector<Block *> blocks;

    // Stores the length of the cube in the world
    real cube_length;

    // Stores the dimensions of the world.
    real min_coord_x;
    real max_coord_x;
    real min_coord_y;
    real max_coord_y;
    real min_coord_z;
    real max_coord_z;

    // This variable is required for when checking for collision in n^2
    map<Block*, CollisionOctree*> block_to_leaf;
    vector<omp_lock_t> collision_locks;

    enum BlockTypes {IBlockType, MBlockType, EBlockType, ZBlockType};


    // Reset both the Octrees -- ForceOctree and CollisionOctree
    void ResetTrees();

    /**
     * @brief Adds a block to the world
     *
     * @param block_types The type of block to add
     * @param num_blocks The number of blocks of this type to add
     * @param state of blocks
     * */
    void AddBlock(BlockTypes block_types, int num_blocks, bool state);

    // Update Flare values of the cubes
    void IncFlareValuesAndReset();

    // Adds orientation to the block, depending upon their flare value
    void AddSpin(vector<Block *> *block_list,  Matrix const &force_direction);

    // Calculate the change in flare value of each block over a given time duration
    void UpdateFlares(vector<Contact> &contact_list, real delta_time);

    // Adds a torque to the blocks
    void SpinWorldBlocks();

public:

    // Stores the different blocks in the world
    vector<IBlock *> iblocks;
    vector<MBlock *> mblocks;
    vector<EBlock *> eblocks;
    vector<ZBlock *> zblocks;

    WorldHandler(int num_i_blocks_plus, int num_i_blocks_neg, int num_z_blocks, int num_m_blocks, int num_e_blocks_1, int num_e_blocks_1_2, real min_coord_x = -50, real max_coord_x = 50, real min_coord_y = -50, real max_coord_y = 50, real min_coord_z = -50, real max_coord_z = 50, real cube_length = 4.0f);

    // Contact generation functions. Returns a list of contacts
    vector<Contact> CollisionHandlerBruteForce();
    vector<Contact> CollisionHandlerWithOctree();

    // Add force to each cube
    void AddForces(real delta_time);

    /**
     * @brief Updates the world by one frame
     *
     * @param contact_list List of contacts that are currently in place
     * @param delta_time The duration for which the update frame should by applied
     * */
    void Update(vector<Contact> &contact_list, real delta_time);

    ~WorldHandler();
};
#endif