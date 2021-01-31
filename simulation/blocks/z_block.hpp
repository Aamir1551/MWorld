#ifndef MWORLD_SIMULATION_BLOCKS_Z_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_Z_BLOCKS_H

#include <block.hpp>

namespace blocks {
    class ZBlock : public Block {

    public:
        ZBlock(Matrix position, Quaternion initial_orientation, real cube_length = 4.0f) : Block(position, initial_orientation, 0.0f, cube_length) {}

        void React(IBlock *block, real dist, const Matrix& to_cube, real deltimatime) override;
        void React(EBlock *block, real dist, const Matrix& to_cube, real deltimatime) override;
        void React(ZBlock *block, real dist, const Matrix& to_cube, real deltimatime) override;
        void React(MBlock *block, real dist, const Matrix& to_cube, real deltimatime) override;

        real ExtractFlareFromBlock(real deltimatime) override;
        void AddFlareToBlock(real flare_amount) override;
    };
}

#endif