#ifndef MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H

#include <block.cpp>

class MBlock : public Block {

public:
    bool state;
    MBlock(Matrix position, Quaternion initial_orientation, bool state) : Block(position, initial_orientation, 10) {
        this->state = state;
    }

};
#endif