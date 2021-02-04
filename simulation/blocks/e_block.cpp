#include <octree.hpp>
#include <block.hpp>
#include <e_block.hpp>
#include <cmath>

namespace blocks {

    real EBlock::ExtractFlareFromBlock(real deltatime) {
        this->flare_inc -= this->flare_value * 0.08 * deltatime; // using the kissing number of spheres
        return this->flare_value * 0.08;
    };

    void EBlock::AddFlareToBlock(real flare_amount) {
        this->flare_inc += (flare_amount) * k;
    }


    bool EBlock::React(Octree * tree, real delta_time) {
        // Attracted to the I+ and E blocks

        if(tree->iblocks_at_cell_plus_count == 0 && tree->eblocks_at_cell_count == 0) {
            return false;
        }

        Matrix vec_to_tree_com_i_plus = tree->com_i_plus - this->position;
        Matrix vec_to_tree_com_e = tree->com_e - this->position;

        real squared_dist_i_plus = Matrix::SquaredNorm(vec_to_tree_com_i_plus);
        real squared_dist_e = Matrix::SquaredNorm(vec_to_tree_com_e);

        real dist_i_plus = std::sqrt(squared_dist_i_plus);
        real dist_e = std::sqrt(squared_dist_e);

        real ratio_i_plus = tree->cell_partition_size / std::max(dist_i_plus, (real) 0.0001);
        real ratio_e = tree->cell_partition_size / std::max(dist_e, (real) 0.0001);

        real theta = 0.2;
        if((ratio_i_plus > theta) && !tree->is_leaf && (ratio_e > theta)) {
            return true;
        }

        if(squared_dist_e >= 25) {
            AddLinearForce(vec_to_tree_com_e / dist_e, Block::force_dt / dist_e * delta_time * ((real) tree->eblocks_at_cell_count));
        }

        if(squared_dist_i_plus >= 25) {
            AddLinearForce(vec_to_tree_com_i_plus / dist_i_plus, Block::force_dt / dist_i_plus  * 1 * delta_time * ((real) tree->iblocks_at_cell_plus_count));
        }

        return false;
    }

};
