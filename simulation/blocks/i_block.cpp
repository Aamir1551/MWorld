#include <cmath>

#include <i_block.hpp>
#include <octree.hpp>

namespace blocks {

    class Block;

    /*bool IBlock::React(ForceOctree * tree, real delta_time) {
        Matrix vec_to_tree_com_i_plus = tree->com_i_plus - this->position;
        Matrix vec_to_tree_com_i_neg = tree->com_i_neg - this->position;
        real squared_dist_i_plus = Matrix::SquaredNorm(vec_to_tree_com_i_plus);
        real squared_dist_i_neg = Matrix::SquaredNorm(vec_to_tree_com_i_plus);
        real dist_i_plus = std::sqrt(squared_dist_i_plus);
        real dist_i_neg = std::sqrt(squared_dist_i_neg);


        if(squared_dist_i_plus> 25) {
            AddLinearForce(vec_to_tree_com_i_plus / dist_i_plus, Block::force_dt / squared_dist_i_plus * -1 * 0.1 * delta_time * (tree->iblocks_at_leaf_plus.size()) * (this->state));
        }

        if(squared_dist_i_neg > 25) {
            AddLinearForce(vec_to_tree_com_i_neg  / dist_i_neg, Block::force_dt / squared_dist_i_neg * -1 * 0.1 * delta_time * (tree->iblocks_at_leaf_neg.size()) * (1-this->state));
        }
    };*/

    real IBlock::ExtractFlareFromBlock(real deltatime) {
        return this->a * deltatime + this->b * (real) (this->flare_value < IBlock::threshold) * deltatime;
    };

    void IBlock::spin(Matrix const &force_direction) {
        this->angular_momentum =  force_direction * (this->a + this->b * (real) (this->flare_value < IBlock::threshold)) * 0.0001;
    }

    void IBlock::AddFlareToBlock(real flare_amount) {
        this->flare_inc += flare_amount * this->b;
    };

    real IBlock::threshold = 2.0f;

    bool IBlock::React(ForceOctree *tree, real delta_time) {
        return false;
    }
};
