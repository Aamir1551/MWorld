#include <block.hpp>
#include <m_block.hpp>

namespace blocks {

    void MBlock::React(IBlock *block, real squared_dist, const Matrix &to_cube) {
        // neutral
    };


    void MBlock::React(EBlock *block, real squared_dist, const Matrix &to_cube) {
        // neutral
    };


    void MBlock::React(MBlock *block, real squared_dist, const Matrix &to_cube) {
        // neutral
    };

    void MBlock::React(ZBlock *block, real squared_dist, const Matrix &to_cube) {
        // ZBlocks are attracted to M+ blocks
        if(this->flare_value > MBlock::threshold) {
            auto &force = to_cube;
            AddTorque(force, this->position, Block::force_dt / squared_dist * 1 ); // as distance increases, force also decreases
        }
    };


    real MBlock::threshold = 50.0f;
    real MBlock::flare_capacity = 50.0f;

};
