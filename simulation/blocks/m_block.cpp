#include <cmath>

#include <block.hpp>
#include <m_block.hpp>
#include <e_block.hpp>
#include <octree.hpp>

namespace blocks {

    void MBlock::AddFlareToBlock(real flare_amount) {
        this->flare_inc += flare_amount;
    }

    void MBlock::Decay(real delta_time) {
       this->flare_value -= (delta_time * EBlock::capacity * 0.08 * 0.55);
    };

    real MBlock::ExtractFlareFromBlock(real deltatime) {
        real extract = (real) (this->flare_value > MBlock::threshold) * this->flare_value * 0.08 * deltatime;
        this->flare_inc -= extract;
        return extract;
    };

    real MBlock::threshold = 0.5f;

    bool MBlock::React(ForceOctree *tree, real delta_time) {
        if (tree->zblocks_at_cell_count == 0) {
            return false;
        }

        Matrix inc_force = Matrix(3, 1);
        bool recurse = ApplyForceFromBlock(tree, delta_time, tree->zblocks_at_cell_count, tree->com_z, inc_force);
        if (recurse == false) {
            inc_force *= (this->flare_value > MBlock::threshold);
            this->AddLinearForce(inc_force, delta_time);
            return false;
        } else {
            return true;
        }
    }
};
