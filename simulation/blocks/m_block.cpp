#include <cmath>

#include <block.hpp>
#include <m_block.hpp>
#include <octree.hpp>

namespace blocks {

    void MBlock::AddFlareToBlock(real flare_amount) {
        this->flare_inc = this->flare_inc + flare_amount;
    }


    real MBlock::ExtractFlareFromBlock(real deltatime) {
        real temp = (real) (this->flare_value > MBlock::threshold) * this->flare_value * 0.08 * deltatime;
        this->flare_inc -= temp;
        return temp;
    };

    real MBlock::threshold = 25.0f;

    bool MBlock::React(ForceOctree *tree, real delta_time) {
        return false;
    }

    /*bool MBlock::React(ForceOctree *tree, real delta_time) {
        Matrix vec_to_tree_com_z = tree->com_z - this->position;
        real squared_dist_z = Matrix::SquaredNorm(vec_to_tree_com_z);
        real dist_z =  std::sqrt(squared_dist_z);

        if(squared_dist_z >= 25) {
            AddLinearForce(vec_to_tree_com_z / dist_z, Block::force_dt / squared_dist_z  * 1 * delta_time * (tree->zblocks_at_leaf.size()));
        }

    }*/
};
