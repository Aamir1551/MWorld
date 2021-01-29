#include <block.hpp>
#include <i_block.hpp>
#include <e_block.hpp>

namespace blocks {

    void IBlock::React(IBlock * block, real squard_dist, const Matrix &to_cube) {
        // I+ and I+ Repel
        // I- and I- Repel
        if(block->state == this->state) {
            // Repel
            auto &force = to_cube;
            AddTorque(force, this->position, Block::force_dt / squard_dist * -1 * 0.1);

        }
    };

    void IBlock::React(MBlock  * block, real squared_dist, const Matrix &to_cube) {

    };

    void IBlock::React(ZBlock  *block, real squared_dist, const Matrix &to_cube) {

    };

    void IBlock::React(EBlock * block, real squared_dist, const Matrix &to_cube) {
        // I+ and E repel
        if(this->state == true) {
            // Repel
            auto &force = to_cube;
            AddTorque(force, this->position, Block::force_dt / squared_dist * 0.1 * -1); // as distance increases, force decreases
        }
    };

    real IBlock::threshold = 2.0f;
};
