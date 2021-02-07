#include <cmath>
#include <i_block.hpp>

namespace blocks {

    class Block;

    real IBlock::ExtractFlareFromBlock(real delta_time) {
        return (this->a + this->b * (real) (this->flare_value < IBlock::threshold)) * 0.05 * delta_time;
    };

    void IBlock::spin(Matrix const &force_direction) {
        this->angular_momentum =  force_direction * (this->a + this->b * (real) (this->flare_value < IBlock::threshold)) * 0.1;
    }

    void IBlock::AddFlareToBlock(real flare_amount) {
        this->flare_inc += flare_amount * this->b;
    };

    void IBlock::Decay(real delta_time) {
        // Decay is needed for the I- Block, since when we do stop extracting flare from other blocks,
        // we still need to put in more flare
        this->flare_value -= delta_time * 0.01; // Make sure amount of flare losing every delta_time second, is less than the amount
        // of flare being passed on by the MBlock every delta_time second.
    }

    real IBlock::threshold = 0.5f;

    bool IBlock::React(ForceOctree *tree, real delta_time) {
        if(this->state) {
           if(tree->iblocks_at_cell_plus_count == 0) {
              return false;
           }
           Matrix inc_force = Matrix(3, 1);
           bool recurse = ApplyForceFromBlock(tree, delta_time, tree->iblocks_at_cell_plus_count, tree->com_i_plus, inc_force);
           if (!recurse) {
               this->AddLinearForce(inc_force * -1, delta_time);
               return false;
           } else {
               return true;
           }
        }  else {
            if(tree->iblocks_at_cell_neg_count == 0) {
                return false;
            }
            Matrix inc_force = Matrix(3, 1);
            bool recurse = ApplyForceFromBlock(tree, delta_time, tree->iblocks_at_cell_neg_count, tree->com_i_neg, inc_force);
            if (!recurse) {
                this->AddLinearForce(inc_force * -1, delta_time);
                return false;
            } else {
                return true;
            }
        }
    }
};
