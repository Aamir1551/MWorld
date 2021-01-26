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
}

