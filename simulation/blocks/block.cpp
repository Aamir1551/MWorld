#ifndef MWORLD_SIMULATION_BLOCK_H
#define MWORLD_SIMULATION_BLOCK_H

#include <cube.cpp>


class Block : public Cube {

public:

    real flare_value = 0.0f;
    real flare_inc = 0.0f;

    Block(Matrix position, Quaternion initial_orientation, real initia_flare_amount) : Cube(1.0f, position,
                                                                                            initial_orientation, 1.0f,
                                                                                            1.0f) {


    };

};

#endif