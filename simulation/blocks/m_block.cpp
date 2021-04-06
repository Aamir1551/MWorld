#include <cmath>

#include <block.hpp>
#include <m_block.hpp>
#include <e_block.hpp>
#include <collision_octree.hpp>

namespace blocks {

    void MBlock::AddFlareToBlock(real flare_amount, Block *b) {
        if(flare_amount > 0) {
            this->flare_inc += flare_amount;
        }
    }

    void MBlock::Decay(real delta_time) {
       this->flare_value = std::max(this->flare_value - 0.5 * 0.08 * delta_time, 0.0);
    };

    void MBlock::UpdateFlare() {
        this->flare_value = std::min((this->flare_inc + this->flare_value), MBlock::capacity);
    };

    real MBlock::ExtractFlareFromBlock(real delta_time) {
        real extract = (real) (this->flare_value > MBlock::threshold) * this->flare_value * 0.08 * delta_time;
        this->flare_inc -= extract;
        return extract;
    };

    real MBlock::threshold = 0.5f;
    real MBlock::capacity = 5;

    bool MBlock::ReactBarnesHut(ForceOctree *tree, real delta_time) {
        // M+ blocks are attracted to the ZBlock
        if (tree->zblocks_at_cell_count == 0) {
            return false;
        }

        Matrix inc_force = Matrix(3, 1);
        bool recurse = ApplyForceFromBlock(tree, tree->zblocks_at_cell_count, tree->com_z, inc_force);
        if (recurse == false) {
            inc_force *= (this->flare_value > MBlock::threshold);
            this->AddLinearForce(inc_force, delta_time);
            return false;
        } else {
            return true;
        }
    }

    void MBlock::ReactSerial(ZBlock *b, real delta_time) {
        // M+ blocks are attracted to the ZBlock
        if(this->flare_value > blocks::MBlock::threshold) {
            Matrix dist_vect = b->position - this->position;
            real squared_dist = Matrix::SquaredNorm(dist_vect);
            if(squared_dist >= 5) {
                this->AddLinearForce(dist_vect/squared_dist, delta_time);
            }
        }
    };
};
