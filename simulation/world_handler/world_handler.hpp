//
// Created by aamir on 24/01/2021.
//

#ifndef MWORLD_SIMULATION_WORLD_HANDLER_H
#define MWORLD_SIMULATION_WORLD_HANDLER_H

#include <cube.hpp>

#include <cstdlib>     /* srand, rand */
#include <ctime>       /* time */

#include <vector>

#include <matrix.hpp>

#include <block.hpp>
#include <i_block.hpp>
#include <m_block.hpp>
#include <e_block.hpp>
#include <z_block.hpp>

using namespace blocks;

class WorldHandler {

public:

    vector<IBlock *> iblocks;
    vector<MBlock *> mblocks;
    vector<EBlock *> eblocks;
    vector<ZBlock *> zblocks;

    vector<Block *> blocks;

    enum BlockTypes {IBlockType, MBlockType, EBlockType, ZBlockType};

    WorldHandler(int num_i_blocks_plus, int num_i_blocks_neg, int num_z_blocks, int num_m_blocks, int num_e_blocks_1, int num_e_blocks_1_2);

    void static GetProperties(int num_blocks, std::vector<Matrix> *&positions, std::vector<Matrix> * &linear_momentums) {
        positions = GeneratePositions(num_blocks);
        linear_momentums = GenerateLinearMomentums(num_blocks);
    }

    void AddBlock(BlockTypes block_types, int num_blocks, bool state);

    void Update();

    void CollisionHandler();

    void AddForces();

    void ReactToAllBlocks(Block *block);

    void PassBlockFlares(vector<Contact> &contacts);

    void static PassFlare(Block *a, Block *b);

    void IncFlareValuesAndReset();

    void AddSpin(vector<Block *> *block_list,  Matrix const &force_direction);

    void SpinWorldBlocks();

    std::vector<Matrix> static *GeneratePositions(int num_cubes)
    {
        auto *positions = new std::vector<Matrix>;
        real world_size = 100.0;
        real const scale = world_size / ((real)RAND_MAX / 2.0);
        auto get_coord = [scale, world_size]() -> real { return scale * (rand() - RAND_MAX / 2.0); };
        for (int i = 0; i < num_cubes; i++)
        {
            real values[] = {get_coord(), get_coord(), get_coord()};
            positions->push_back(Matrix(3, 1, values));
        }
        return positions;
    }

    std::vector<Matrix> static *GenerateLinearMomentums(int num_cubes)
    {
        auto *linear_momentums = new std::vector<Matrix>;
        auto get_momentums = []() -> real { return (rand() % 1)/100 - 0.0005;};
        for (int i = 0; i < num_cubes; i++)
        {
            real values[] = { get_momentums() , get_momentums() , get_momentums()};
            linear_momentums->push_back(Matrix(3, 1, values));
        }
        return linear_momentums;
    }

};
#endif