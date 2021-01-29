#ifndef MWORLD_SIMULATION_BLOCKS_Z_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_Z_BLOCKS_H

#include <block.hpp>

namespace blocks {
    class ZBlock : public Block {

    public:
        ZBlock(Matrix position, Quaternion initial_orientation) : Block(position, initial_orientation, 0.0f) {}

        void React(IBlock *block, real dist, const Matrix& to_cube) override;
        void React(EBlock *block, real dist, const Matrix& to_cube) override;
        void React(ZBlock *block, real dist, const Matrix& to_cube) override;
        void React(MBlock *block, real dist, const Matrix& to_cube) override;


        real ExtractFlareFromBlock() override {
            return 0.0;
        };

        void AddFlareToBlock(real flare_amount) override {};
    };
}

#endif