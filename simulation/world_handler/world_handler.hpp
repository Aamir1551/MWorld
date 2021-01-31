#ifndef MWORLD_SIMULATION_WORLD_HANDLER_H
#define MWORLD_SIMULATION_WORLD_HANDLER_H

#include <cstdlib>
#include <ctime>

#include <vector>

#include <matrix.hpp>

#include <cube.hpp>
#include <block.hpp>
#include <i_block.hpp>
#include <m_block.hpp>
#include <e_block.hpp>
#include <z_block.hpp>

using namespace blocks;

class WorldHandler {

private:
    void static GetProperties(int num_blocks, std::vector<Matrix> *&positions, std::vector<Matrix> * &linear_momentums);
    std::vector<Matrix> static *GeneratePositions(int num_cubes);
    std::vector<Matrix> static *GenerateLinearMomentums(int num_cubes);
    void static PassBlockFlares(vector<Contact> &contacts, real deltatime);
    void static PassFlare(Block *a, Block *b, real deltatime);

public:
    vector<IBlock *> iblocks;
    vector<MBlock *> mblocks;
    vector<EBlock *> eblocks;
    vector<ZBlock *> zblocks;
    vector<Block *> blocks;

    enum BlockTypes {IBlockType, MBlockType, EBlockType, ZBlockType};

    WorldHandler(int num_i_blocks_plus, int num_i_blocks_neg, int num_z_blocks, int num_m_blocks, int num_e_blocks_1, int num_e_blocks_1_2);

    void AddBlock(BlockTypes block_types, int num_blocks, bool state);

    void Update();

    void CollisionHandler(real deltatime);

    void AddForces(real deltatime);

    void ReactToAllBlocks(Block *block, real deltatime);

    void IncFlareValuesAndReset();

    void AddSpin(vector<Block *> *block_list,  Matrix const &force_direction);

    void SpinWorldBlocks();
};
#endif