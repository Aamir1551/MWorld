#ifndef MWORLD_SIMULATION_BLOCKS_I_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_I_BLOCKS_H

#include <block.cpp>

class Block;

class IBlock;
class EBlock;
class ZBlock;
class MBlock;

class IBlock : public Block {

public:
    bool state; //state = true, means its a block+
    IBlock(Matrix position, Quaternion initial_orientation, bool state) : Block(position, initial_orientation, 10) {
        this->state = state;
    }

    void React(IBlock *block) override {

    };
    void React(MBlock *block) override {

    };
    void React(ZBlock *block) override {

    };
    void React(EBlock *block) override {

    };

};
#endif