#include <block.hpp>
#include <e_block.hpp>
#include <i_block.hpp>
#include <cmath>

namespace blocks {

    void EBlock::React(IBlock * block, real dist, const Matrix& to_cube) {
        if(block->state == true) {
            // E block and i+ block repel
            auto &force = to_cube;
            AddTorque(force, this->position, Block::force_dt / dist * 0.1 * -1); // as distance increases, force also decreases
        }
    };

    //virtual void React(IBlock * const block, real dist, const Matrix& to_cube) = 0;
    void EBlock::React(MBlock * block, real dist, const Matrix& to_cube) {
        // neutral
    };

    void EBlock::React(ZBlock * block, real dist, const Matrix& to_cube) {
        // neutral
    };

    void EBlock::React(EBlock * block, real dist, const Matrix& to_cube) {
        // E block and E block repel as k increases
        float factor = std::log(block->k * this->k); // is zero is both blocks are k=1
        auto &force = to_cube;
        AddTorque(force, this->position, force_dt * factor / dist * -1 * 0.1);
    };


    real EBlock::ExtractFlareFromBlock() {
        this->flare_inc -= this->flare_value * 0.1;
        return this->flare_value * 0.1;
    };

    void EBlock::AddFlareToBlock(real flare_amount) {
        this->flare_inc += (flare_amount * 0.1) * k;
    };
};
