#include <block.hpp>
#include <e_block.hpp>
#include <i_block.hpp>

namespace blocks {

    void EBlock::React(IBlock *block) {
        if(block->state == true) {
            auto force =  (block->position - this->position);
            force /= Matrix::Norm(force);
            AddTorque(block->position - this->position, );
        };
    };

    void EBlock::React(MBlock *block) {

    };

    void EBlock::React(ZBlock *block) {

    };

    void EBlock::React(EBlock *block) {

    };


    real EBlock::ExtractFlareFromBlock() {
        this->flare_inc -= this->flare_value * 0.1;
        return this->flare_value * 0.1;
    };

    void EBlock::AddFlareToBlock(real flare_amount) {
        this->flare_inc += (flare_amount * 0.1) * k;
    };
};
