#ifndef MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H

#include <block.cpp>

class Block;

class IBlock;
class MBlock;
class ZBlock;
class EBlock;
class MBlock : public Block {

public:
    bool state;
    MBlock(Matrix position, Quaternion initial_orientation, bool state) : Block(position, initial_orientation, 10) {
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

    void Interact(IBlock *block) override {

    };
    void Interact(MBlock *block) override {

    };
    void Interact(ZBlock *block) override {

    };
    void Interact(EBlock *block) override {

    };
};
#endif