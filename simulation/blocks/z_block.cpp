#include <cmath>

#include <block.hpp>
#include <z_block.hpp>
#include <octree.hpp>

namespace blocks {



    void ZBlock::React(Octree * tree, real delta_time) {
        Matrix vec_to_tree_com_z = tree->com_z - this->position;
        Matrix vec_to_tree_com_m_plus = tree->com_m_plus - this->position;

        real squared_dist_z = Matrix::SquaredNorm(vec_to_tree_com_z);
        real squared_dist_m_plus = Matrix::SquaredNorm(vec_to_tree_com_m_plus);


        real dist_z =  std::sqrt(squared_dist_z);
        real dist_m_plus =  std::sqrt(squared_dist_m_plus);

        if(squared_dist_z >= 25) {
            AddLinearForce(vec_to_tree_com_z/dist_z, Block::force_dt / dist_z * 1 * delta_time * (tree->zblocks_at_leaf.size()));
        }

        if(dist_m_plus >= 25) {
            AddLinearForce(vec_to_tree_com_m_plus / dist_m_plus, Block::force_dt / dist_m_plus  * 1 * delta_time * (tree->mblocks_at_leaf_plus.size()));
        }
    }

    real ZBlock::ExtractFlareFromBlock(real deltimatime) {
        return 0.0;
    };

    void ZBlock::AddFlareToBlock(real flare_amount) {};
}
