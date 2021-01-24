#ifndef MWORLD_SIMULATION_BLOCK_H
#define MWORLD_SIMULATION_BLOCK_H

#include <cube.cpp>


#include <e_block.cpp>
#include <i_block.cpp>
#include <m_block.cpp>
#include <z_block.cpp>

class Block : public Cube {

public:

    real flare_value = 0.0f;
    real flare_inc = 0.0f;

    Block(Matrix position, Quaternion initial_orientation, real initial_flare_amount) : Cube(1.0f, position,
                                                                                            initial_orientation, 1.0f,
                                                                                            1.0f) {
        this->flare_value = initial_flare_amount;

    };

    void SetLinearMomentum(Matrix &linear_momentum) {
        this->momentum = linear_momentum;
    }

    void SetAngularMomentum(Matrix &angular_momentum) {
        this->angular_momentum = angular_momentum;
    }

    //when another block is closest to you
    //void virtual React(Block block) = 0;

    void virtual React(IBlock block) = 0;
    void virtual React(MBlock block) = 0;
    void virtual React(ZBlock block) = 0;
    void virtual React(EBlock block) = 0;

    //when another block is touching you
    //void virtual Interact(Block block) = 0;

};

#endif