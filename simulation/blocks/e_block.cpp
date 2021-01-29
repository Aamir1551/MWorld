#include <block.hpp>
#include <e_block.hpp>
#include <i_block.hpp>
#include <cmath>

namespace blocks {

    void EBlock::React(IBlock * block, real squared_dist, const Matrix& to_cube) {
        // I+ and E repel
        if(block->state == true) {
            // If k > 1: Repel, Otherwise Attract
            real factor = std::log(this->k);
            auto &force = to_cube;
            //AddTorque(force, this->position, Block::force_dt / squared_dist * 0.1 * -1 * factor); // as distance increases, force decreases
            AddLinearForce(force, Block::force_dt / squared_dist * 0.1 * -1 * factor);
        }
    };

    void EBlock::React(MBlock * block, real squared_dist, const Matrix& to_cube) {
        // neutral
    };

    void EBlock::React(ZBlock * block, real squared_dist, const Matrix& to_cube) {
        // neutral
    };

    void EBlock::React(EBlock * block, real squared_dist, const Matrix& to_cube) {
        // E block and E block repel as k increases
        float factor = std::log(block->k * this->k); // is zero is both blocks are k=1
        auto &force = to_cube;
        //AddTorque(force, this->position, force_dt * factor / squared_dist * -1 * 0.1);
        AddLinearForce(force, force_dt * factor / squared_dist * -1 * 0.1);
    };


    real EBlock::ExtractFlareFromBlock() {
        this->flare_inc -= this->flare_value * 0.08; // using the kissing number of spheres
        return this->flare_value * 0.08;
    };

    void EBlock::AddFlareToBlock(real flare_amount) {
        this->flare_inc += (flare_amount * 0.1) * k;
    };

    real EBlock::flare_capacity = 50.0f;
};
