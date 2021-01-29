#include <matrix.hpp>
#include <block.hpp>

using namespace numerics;

namespace blocks {

    void Block::SetLinearMomentum(Matrix &linear_momentum) {
        this->momentum = linear_momentum;
    }

    void Block::SetAngularMomentum(Matrix &angular_momentum) {
        this->angular_momentum = angular_momentum;
    }

    real Block::force_dt = 0.01;
    real Block::flare_capacity = 50.0f;
}

