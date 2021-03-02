#include <cmath>
#include <i_block.hpp>

namespace blocks {

    class Block;

    real IBlock::ExtractFlareFromBlock(real delta_time) {
        return (this->a + this->b * (real) (this->flare_value < this->threshold)) * 0.5 * delta_time;
    };

    void IBlock::spin(Matrix const &force_direction) {
        this->angular_momentum =  force_direction * (this->a + this->b * (real) (this->flare_value < IBlock::threshold)) * 0.1;
    }

    void IBlock::AddFlareToBlock(real flare_amount, Block * b) {
        if(flare_amount < 0) {
            this->flare_inc +=  flare_amount * -1;
        }
    };

    void IBlock::Decay(real delta_time) {
        this->flare_value = std::max(this->flare_value - 0.08 * delta_time * 1.5 * 0.5, 0.0);
    }

    real IBlock::threshold = 0.1f;

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
