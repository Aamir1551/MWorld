#include <cmath>

#include <matrix.hpp>
#include <block.hpp>
#include <force_octree.hpp>

using namespace numerics;

namespace blocks {

    void Block::SetLinearMomentum(Matrix &linear_momentum) {
        this->momentum = linear_momentum;
    }

    void Block::SetAngularMomentum(Matrix &angular_momentum) {
        this->angular_momentum = angular_momentum;
    }

    bool Block::ApplyForceFromBlock(ForceOctree* tree, int cell_count, const Matrix &com, Matrix &inc_force) {

        if(cell_count != 0) {
            Matrix vec_to_tree_com = com - this->position;
            real squared_dist = Matrix::SquaredNorm(vec_to_tree_com);
            real dist = std::sqrt(squared_dist);

            if(tree->is_leaf) {
                if(squared_dist >= 5) {
                    inc_force += (vec_to_tree_com) / squared_dist * ((real) cell_count);
                }
                return false;
            }

            real ratio =  tree->cell_partition_size / std::max(dist, (real) 0.0001);
            if((ratio > Block::theta) && !tree->is_leaf) {
                return true;
            }

            if(squared_dist >= 5) {
                inc_force += (vec_to_tree_com) / squared_dist * ((real) cell_count);
            }
        }
        return false;
    }


    void Block::spin(Matrix const &force_direction) {
        this->angular_momentum = force_direction * this->flare_value * 0.005;
    }

    void Block::UpdateFlare() {
        this->flare_value = std::max(std::min( (this->flare_inc + this->flare_value), Block::flare_capacity), 0.0f);
    };

    real Block::flare_capacity = 1.0f;
    real Block::theta = 0.5f;

    void Block::Decay(real delta_time) {};

    void Block::ReactLinear(IBlock *b, real delta_time){};
    void Block::ReactLinear(MBlock *b, real delta_time){};
    void Block::ReactLinear(ZBlock *b, real delta_time){};
    void Block::ReactLinear(EBlock *b, real delta_time){};

}

