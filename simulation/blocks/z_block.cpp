#include <cmath>

#include <block.hpp>
#include <z_block.hpp>
#include <octree.hpp>
#include <force_octree.hpp>

namespace blocks {

    bool ZBlock::React(ForceOctree * tree, real delta_time) {

        if(tree->zblocks_at_cell_count == 0 && tree->mblocks_at_cell_plus_count == 0){
            return false;
        }

        Matrix vec_to_tree_com_z = tree->com_z - this->position;
        Matrix vec_to_tree_com_m_plus = tree->com_m_plus - this->position;

        real squared_dist_z = Matrix::SquaredNorm(vec_to_tree_com_z);
        real squared_dist_m_plus = Matrix::SquaredNorm(vec_to_tree_com_m_plus);

        real dist_z =  std::sqrt(squared_dist_z);
        real dist_m_plus =  std::sqrt(squared_dist_m_plus);


        //real ratio_z =  tree->cell_partition_size / std::max(dist_z, (real) 0.0001);
        real ratio_z =  tree->cell_partition_size / dist_z;
        real ratio_m_plus = tree->cell_partition_size / std::max(dist_m_plus, (real) 0.00001);

        real theta = 0.2;
        if((ratio_z > theta) && !tree->is_leaf) {
           return true;
        }

        if(squared_dist_z == 0) {
            if(!tree->is_leaf && tree->zblocks_at_cell_count > 1) {
                return true;
            } else {
                return false;
            }
        }


        if(squared_dist_z >= 25) {
            AddLinearForce(vec_to_tree_com_z/dist_z, Block::force_dt / squared_dist_z * 1000 * delta_time * ((real) tree->zblocks_at_cell_count ));
        }

        /*if(dist_m_plus >= 25) {
            AddLinearForce(vec_to_tree_com_m_plus / dist_m_plus, Block::force_dt / dist_m_plus  * 1 * delta_time * ((real) tree->mblocks_at_cell_plus_count));
        }*/
        return false;
    }

    real ZBlock::ExtractFlareFromBlock(real deltimatime) {
        return 0.0;
    };

    void ZBlock::AddFlareToBlock(real flare_amount) {};
}


/*

#include <cmath>

#include <block.hpp>
#include <z_block.hpp>
#include <octree.hpp>

namespace blocks {

    bool ZBlock::React(Octree * tree, real delta_time) {

        if(tree->zblocks_at_cell_count == 0 && tree->mblocks_at_cell_plus_count == 0){
            return false;
        }

        Matrix vec_to_tree_com_z = tree->com_z - this->position;
        Matrix vec_to_tree_com_m_plus = tree->com_m_plus - this->position;

        real squared_dist_z = Matrix::SquaredNorm(vec_to_tree_com_z);
        real squared_dist_m_plus = Matrix::SquaredNorm(vec_to_tree_com_m_plus);

        real dist_z =  std::sqrt(squared_dist_z);
        real dist_m_plus =  std::sqrt(squared_dist_m_plus);

        real ratio_z =  tree->cell_partition_size / std::max(dist_z, (real) 0.0001);
        real ratio_m_plus = tree->cell_partition_size / std::max(dist_m_plus, (real) 0.0001);

        real theta = 0.0;

        if((ratio_z > theta) && !tree->is_leaf) {
            return true;
        }
       // /*if((ratio_z > theta) && !tree->is_leaf && (ratio_m_plus > theta)) {
       //    return true;
     //   }

        if(squared_dist_z >= 25) {
            AddLinearForce(vec_to_tree_com_z / dist_z, Block::force_dt / dist_z * delta_time * ((real) tree->zblocks_at_cell_count ));
        }

        ///*if(squared_dist_m_plus >= 25) {
        //    AddLinearForce(vec_to_tree_com_m_plus / dist_m_plus, Block::force_dt / dist_m_plus  * 1 * delta_time * ((real) tree->mblocks_at_cell_plus_count));
        //}

        return false;
    }

    real ZBlock::ExtractFlareFromBlock(real deltimatime) {
        return 0.0;
    };

    void ZBlock::AddFlareToBlock(real flare_amount) {};
}

*/