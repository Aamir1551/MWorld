#include <block.hpp>
#include <m_block.hpp>

namespace blocks {

    void MBlock::React(IBlock *block, real squared_dist, const Matrix &to_cube, real deltatime) {
        // neutral
    };


    void MBlock::React(EBlock *block, real squared_dist, const Matrix &to_cube, real deltatime) {
        // neutral
    };


    void MBlock::React(MBlock *block, real squared_dist, const Matrix &to_cube, real deltatime) {
        // neutral
    };

    void MBlock::React(ZBlock *block, real squared_dist, const Matrix &to_cube, real deltatime) {
        // ZBlocks are attracted to M+ blocks
        if(this->flare_value > MBlock::threshold) {
            auto &force = to_cube;
            AddLinearForce(force,  Block::force_dt / squared_dist * 1); // as distance increases, force also decreases
        }
    };

    void MBlock::AddFlareToBlock(real flare_amount) {
        this->flare_inc = this->flare_inc + flare_amount;
    }


    real MBlock::ExtractFlareFromBlock(real deltatime) {
        real temp = (real) (this->flare_value > MBlock::threshold) * this->flare_value * 0.08;
        this->flare_inc -= temp;
        return temp;
    };

    real MBlock::threshold = 25.0f;
};
