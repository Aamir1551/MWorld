#include <block.hpp>
#include <m_block.hpp>

namespace blocks {

    void MBlock::React(IBlock *block, real dist, const Matrix &to_cube) {
        // neautral
    };


    void MBlock::React(EBlock *block, real dist, const Matrix &to_cube) {
        // neautral
    };


    void MBlock::React(MBlock *block, real dist, const Matrix &to_cube) {
        // neautral
        /*if(this->state != block->state) {
            auto &force = to_cube;
            AddTorque(force, this->position, Block::force_dt / dist / dist * -0.1); // as distance increases, force decreases
        }*/
    };

    void MBlock::React(ZBlock *block, real dist, const Matrix &to_cube) {
        // neautral
    };


    real MBlock::threshold = 5.0f;
    real MBlock::flare_capacity = 5.0f;

};
