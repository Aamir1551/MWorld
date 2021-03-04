#include <octree.hpp>
#include <force_octree.hpp>
#include <block.hpp>
#include <e_block.hpp>


namespace blocks {

    real EBlock::ExtractFlareFromBlock(real delta_time) {
        real extract_flare = this->flare_value * 0.08 * delta_time * k;
        this->flare_inc -= extract_flare; // using the kissing number of spheres
        return extract_flare;
    };

    void EBlock::AddFlareToBlock(real flare_amount, Block *b) {
        if(flare_amount > 0) {
            this->flare_inc += flare_amount;
        }
    }

    void EBlock::UpdateFlare() {
        this->flare_value = std::min(this->flare_inc + this->flare_value, EBlock::capacity);
    };

    bool EBlock::React(ForceOctree * tree, real delta_time) {
        // Attracted to E blocks
        if (tree->eblocks_at_cell_count == 0) {
            return false;
        }

        Matrix inc_force = Matrix(3, 1);
        bool recurse = ApplyForceFromBlock(tree, delta_time, tree->eblocks_at_cell_count, tree->com_e, inc_force);
        if (recurse == false) {
            this->AddLinearForce(inc_force *1 , delta_time);
            return false;
        } else {
            return true;
        };
    }

    real EBlock::capacity = 0.5;
};
