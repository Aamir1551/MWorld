#ifndef MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H

#include <block.hpp>

namespace blocks {

    class MBlock : public Block {

    public:
        static real threshold;
        static real flare_capacity;

        MBlock(Matrix position, Quaternion initial_orientation) : Block(position, initial_orientation, 0) {
        }

        void React(IBlock *block, real dist, const Matrix& to_cube) override;
        void React(MBlock *block, real dist, const Matrix& to_cube) override;
        void React(EBlock *block, real dist, const Matrix& to_cube) override;
        void React(ZBlock *block, real dist, const Matrix& to_cube) override;


        real ExtractFlareFromBlock() override {
            real temp = (real) (this->flare_value > MBlock::threshold) * this->flare_value * 0.1;
            this->flare_inc -= temp;
            return temp;
        };

        void AddFlareToBlock(real flare_amount) override {
            this->flare_inc = this->flare_inc + flare_amount * 0.1;
        };

        void UpdateFlare() override {
            this->flare_value = std::max(this->flare_inc + this->flare_value, MBlock::flare_capacity);
        };

    };

}

#endif