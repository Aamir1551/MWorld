#include <cmath>

#include <block.hpp>
#include <z_block.hpp>
#include <octree.hpp>
#include <force_octree.hpp>

namespace blocks {

    bool ZBlock::React(ForceOctree *tree, real delta_time) {

        if (tree->zblocks_at_cell_count == 0 && tree->mblocks_at_cell_plus_count == 0) {
            return false;
        }

        Matrix inc_force = Matrix(3, 1);
        bool recurse = ApplyForceFromBlock(tree, delta_time, tree->zblocks_at_cell_count, tree->com_z, inc_force);
        if (recurse == false) {
            recurse = ApplyForceFromBlock(tree, delta_time, tree->mblocks_at_cell_plus_count, tree->com_m_plus,
                                          inc_force);
        }
        if (!recurse) {
            this->AddLinearForce(inc_force, delta_time);
            return false;
        } else {
            return true;
        }
    }

    void ZBlock::Decay(real delta_time) {
        this->flare_value -= delta_time * 0.02;
    }

    real ZBlock::ExtractFlareFromBlock(real delta_time) {
        if(flare_value > 0.5) {
            real extract_flare = this->flare_value * 0.08 * delta_time;
            this->flare_inc -= extract_flare; // using the kissing number of spheres
            return extract_flare * -1;
        }
    };

    void ZBlock::AddFlareToBlock(real flare_amount, Block *b) {
        if(b->block_type!=0) {
            this->flare_inc += (flare_amount) * 3;
        }
    }

    void ZBlock::UpdateFlare() {
        this->flare_value = std::max(std::min(this->flare_inc + this->flare_value, (real) 1), (real) -1);
    };
}
