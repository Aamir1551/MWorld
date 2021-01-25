#ifndef MWORLD_SIMULATION_BLOCK_H
#define MWORLD_SIMULATION_BLOCK_H

#include <cube.cpp>

/*#include <i_block.cpp>
#include <e_block.cpp>
#include <m_block.cpp>
#include <z_block.cpp>*/

class IBlock;
class EBlock;
class MBlock;
class ZBlock;

class Block : public Cube {

public:

    real flare_value = 0.0f;
    real flare_inc = 0.0f;

    Block(Matrix position, Quaternion initial_orientation, real initial_flare_amount) : Cube(4.0f, position,
                                                                                            initial_orientation, 1.0f,
                                                                                            1.0f) {
        this->flare_value = initial_flare_amount;

    };

    void SetLinearMomentum(Matrix &linear_momentum);

    void SetAngularMomentum(Matrix &angular_momentum);

    //when another block is closest to you

    virtual void React(IBlock *block) = 0;
    virtual void React(MBlock *block) = 0;

    virtual void React(ZBlock *block) = 0;

    virtual void React(EBlock *block) = 0;

    //when another block is touching you
    virtual void Interact(IBlock *block) = 0;
    virtual void Interact(MBlock *block) = 0;
    virtual void Interact(ZBlock *block) = 0;
    virtual void Interact(EBlock *block) = 0;

};

void Block::SetLinearMomentum(Matrix &linear_momentum) {
    this->momentum = linear_momentum;
}

void Block::SetAngularMomentum(Matrix &angular_momentum) {
    this->angular_momentum = angular_momentum;
}
#endif