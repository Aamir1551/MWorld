#include <block.hpp>
#include <i_block.hpp>
#include <e_block.hpp>
#include <cmath>

namespace blocks {

    void IBlock::React(IBlock * block, real squared_dist, const Matrix &to_cube) {
        // I+ and I+ Repel
        // I- and I- Repel
        if(block->state == this->state) {
            // Repel
            auto &force = to_cube;
            //AddTorque(force, this->position, Block::force_dt / squard_dist * -1 * 0.1);
            AddLinearForce(force, Block::force_dt / squared_dist * -1 * 0.1);
        }
    };

    void IBlock::React(MBlock  * block, real squared_dist, const Matrix &to_cube) {
        // neutral
    };

    void IBlock::React(ZBlock  *block, real squared_dist, const Matrix &to_cube) {
        // neutral
    };

    void IBlock::React(EBlock * block, real squared_dist, const Matrix &to_cube) {
        // I+ and E repel
        if(this->state == true) {
            // If k > 1: Repel, Otherwise Attract
            real factor = std::log(block->k);
            auto &force = to_cube;
            AddLinearForce(force, Block::force_dt / squared_dist * 0.1 * -1 * factor);
            //AddTorque(force, this->position, Block::force_dt / squared_dist * 0.1 * -1 * factor); // as distance increases, force decreases
        }
    };

    real IBlock::threshold = 2.0f;
};
