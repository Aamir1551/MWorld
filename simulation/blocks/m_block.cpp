#include <cmath>

#include <block.hpp>
#include <m_block.hpp>
#include <e_block.hpp>
#include <octree.hpp>

namespace blocks {

    void MBlock::AddFlareToBlock(real flare_amount, Block *b) {
        if(flare_amount > 0) {
            this->flare_inc += flare_amount;
        }
    }

    void MBlock::Decay(real delta_time) {
       //this->flare_value -= (delta_time * EBlock::capacity * 0.08 * 0.55);
       this->flare_value -= 0.5 * 0.08 * delta_time * (this->flare_value/4) * (this->flare_value/4);

       // if they have less flare, then we decay less

       // we need decay to be more than 0.5 * 0.08
       //this->flare_value -= delta_time * 0.001;
       this->flare_value = std::max(this->flare_value, (real) 0);
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
