#include <block.hpp>
#include <i_block.hpp>
#include <e_block.hpp>

namespace blocks {

    void IBlock::React(IBlock * block, real dist, const Matrix &to_cube) {
        if(block->state == this->state) {
            // Repel
            auto &force = to_cube;
            AddTorque(force, this->position, Block::force_dt / dist / dist * -1); // as distance increases, force also decreases
        }
    };

    void IBlock::React(MBlock  * block, real dist, const Matrix &to_cube) {

    };

    void IBlock::React(ZBlock  *block, real dist, const Matrix &to_cube) {

    };

    void IBlock::React(EBlock * block, real dist, const Matrix &to_cube) {
        if(this->state == true) {
            auto &force = to_cube;
            AddTorque(force, this->position, Block::force_dt / dist / dist); // as distance increases, force decreases
        }
    };

    real IBlock::threshold = 2.0f;
};
