#include <block.hpp>
#include <i_block.hpp>
#include <e_block.hpp>
#include <cmath>

namespace blocks {

    void IBlock::React(IBlock * block, real squared_dist, const Matrix &to_cube, real deltatime) {
        // I+ and I+ Repel
        // I- and I- Repel
        if(block->state == this->state) {
            // Repel
            auto &force = to_cube;
            AddLinearForce(force, Block::force_dt / squared_dist * -1 * 0.1 * deltatime);
        }
    };

    void IBlock::React(MBlock  * block, real squared_dist, const Matrix &to_cube, real deltatime) {
        // neutral
    };

    void IBlock::React(ZBlock  *block, real squared_dist, const Matrix &to_cube, real deltatime) {
        // neutral
    };

    void IBlock::React(EBlock * block, real squared_dist, const Matrix &to_cube, real deltatime) {
        // I+ and E repel
        if(this->state == true) {
            // If k > 1: Repel, Otherwise Attract
            real factor = std::log(block->k);
            auto &force = to_cube;
            AddLinearForce(force, Block::force_dt / squared_dist * 0.1 * -1 * factor * deltatime);
        }
    };


    real IBlock::ExtractFlareFromBlock(real deltatime) {
        return this->a * deltatime + this->b * (real) (this->flare_value < IBlock::threshold) * deltatime;
    };

    void IBlock::spin(Matrix const &force_direction) {
        this->angular_momentum =  force_direction * (this->a + this->b * (real) (this->flare_value < IBlock::threshold)) * 0.0001;
    }

    void IBlock::AddFlareToBlock(real flare_amount) {
        this->flare_inc += flare_amount * this->b;
    };

    real IBlock::threshold = 2.0f;
};
