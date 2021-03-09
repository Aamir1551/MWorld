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

#include <octree.hpp>
#include <force_octree.hpp>

using namespace blocks;

class WorldHandler {

private:
    void static GetProperties(int num_blocks, std::vector<Matrix> *&positions, std::vector<Matrix> * &linear_momentums, real min_coord, real max_coord);
    std::vector<Matrix> static *GeneratePositions(int num_cubes, real min_coord, real max_coord);
    std::vector<Matrix> static *GenerateLinearMomentums(int num_cubes);
    void static PassBlockFlares(vector<Contact> &contacts, real deltatime);
    void static PassFlare(Block *a, Block *b, real deltatime);

    template<typename T>
    std::unordered_set<T> static VecToSetParallel(vector<T> v);

    std::unordered_set<Octree*> GetBlockPositionsParallel();
    std::unordered_set<Octree*> GetBlockPositionsSerial();

    void MakeDAG(std::unordered_set<Octree *> &block_positions, std::vector<pair<Octree *, Octree *>> &edges, std::unordered_set<Octree *> &nodes);
    void GetDAGAtRoot(Octree *root, std::vector<pair<Octree *, Octree *>> &edges, std::unordered_set<Octree *> &nodes);

public:
    vector<IBlock *> iblocks;
    vector<MBlock *> mblocks;
    vector<EBlock *> eblocks;
    vector<ZBlock *> zblocks;

    Octree *tree;
    ForceOctree *forces_tree;
    vector<Block *> blocks;
    real world_size;
    real cube_length;
    real min_coord;
    real max_coord;
    map<Block*, Octree*> block_to_leaf; // This type is only needed for when checking for collision in n^2 fashion
    vector<omp_lock_t> collision_locks;


    enum BlockTypes {IBlockType, MBlockType, EBlockType, ZBlockType};

    WorldHandler(int num_i_blocks_plus, int num_i_blocks_neg, int num_z_blocks, int num_m_blocks, int num_e_blocks_1, int num_e_blocks_1_2, real min_coord = - 50, real max_coord = 50, real cube_length = 4.0f);

    void ResetTrees();

    void AddBlock(BlockTypes block_types, int num_blocks, bool state);

    void Update(vector<Contact> &contact_list, real delta_time);

    vector<Contact> CollisionHandlerBruteForce();
    vector<Contact> CollisionHandlerWithOctree();

    void AddForces(real deltatime);

    void IncFlareValuesAndReset();

    void AddSpin(vector<Block *> *block_list,  Matrix const &force_direction);

    void UpdateFlares(vector<Contact> &contact_list, real delta_time);

    void SpinWorldBlocks();

    ~WorldHandler();
};
#endif