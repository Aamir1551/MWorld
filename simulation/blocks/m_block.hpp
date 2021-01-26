#ifndef MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H

#include <block.hpp>

namespace blocks {

    class MBlock : public Block {

    public:
        bool state;
        static real threshold;
        static real flare_capacity;

        MBlock(Matrix position, Quaternion initial_orientation, bool state) : Block(position, initial_orientation, 10) {
            this->state = state;
        }


        void React(IBlock *block) override;

        void React(MBlock *block) override;

        void React(ZBlock *block) override;

        void React(EBlock *block) override;

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