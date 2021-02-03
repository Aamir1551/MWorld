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

    void EBlock::React(Octree *tree, real delta_time) {
        // Attracted to the I+ and E blocks

        Matrix vec_to_tree_com_i_plus = tree->com_i_plus - this->position;
        Matrix vec_to_tree_com_e = tree->com_e - this->position;

        real squared_dist_i_plus = Matrix::SquaredNorm(vec_to_tree_com_i_plus);
        real squared_dist_e = Matrix::SquaredNorm(vec_to_tree_com_e);

        real dist_i_plus =  std::sqrt(squared_dist_i_plus);
        real dist_e =  std::sqrt(squared_dist_e);


        if(squared_dist_e >= 25) {
            AddLinearForce(vec_to_tree_com_e / dist_e, Block::force_dt / squared_dist_e  * 1 * delta_time * (tree->eblocks_at_leaf.size()));
        }

        if(squared_dist_i_plus >= 25) {
            AddLinearForce(vec_to_tree_com_i_plus / dist_i_plus, Block::force_dt / squared_dist_i_plus  * 1 * delta_time * (tree->iblocks_at_leaf_plus.size()));
        }

    };
};
