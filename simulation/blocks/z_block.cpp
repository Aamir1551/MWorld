#include <cmath>

#include <block.hpp>
#include <z_block.hpp>
#include <collision_octree.hpp>
#include <force_octree.hpp>

namespace blocks {

    bool ZBlock::ReactBarnesHut(ForceOctree *tree, real delta_time) {
        // Attracted to Both MBlocks an ZBlocks

        if (tree->zblocks_at_cell_count == 0 && tree->mblocks_at_cell_plus_count == 0) {
            return false;
        }

        Matrix inc_force = Matrix(3, 1);
        bool recurse = ApplyForceFromBlock(tree, tree->zblocks_at_cell_count, tree->com_z, inc_force);
        if (recurse == false) {
            recurse = ApplyForceFromBlock(tree, tree->mblocks_at_cell_plus_count, tree->com_m_plus,
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
        this->flare_value = std::max(this->flare_value - 0.02 * delta_time , 0.0);
    }

    real ZBlock::ExtractFlareFromBlock(real delta_time) {
        if(flare_value > ZBlock::threshold) {
            real extract_flare = this->flare_value * 0.08 * delta_time;
            this->flare_inc -= extract_flare; // using the kissing number of spheres
            return extract_flare * -1;
        }
        return 0;
    };

    void ZBlock::AddFlareToBlock(real flare_amount, Block *b) {
        if(b->block_type!=0) {
            this->flare_inc += (flare_amount);
        }
    }

    void ZBlock::UpdateFlare() {
        this->flare_value = std::min(this->flare_inc + this->flare_value, (real) 1);
    }

    void ZBlock::ReactSerial(ZBlock *b, real delta_time) {
        // Attracted to the ZBlock
        Matrix dist_vect = b->position - this->position;
        real squared_dist = Matrix::SquaredNorm(dist_vect);
        if(squared_dist >= 5) {
            this->AddLinearForce(dist_vect/squared_dist, delta_time);
        }
    };

    void ZBlock::ReactSerial(MBlock *b, real delta_time) {
        // Attracted to the MBlock
        if(b->flare_value >= blocks::MBlock::capacity) {
            Matrix dist_vect = b->position - this->position;
            real squared_dist = Matrix::SquaredNorm(dist_vect);
            if(squared_dist >= 5) {
                this->AddLinearForce(dist_vect/squared_dist, delta_time);
            }
        }
    };

    real ZBlock::threshold = 0.5f;
}
