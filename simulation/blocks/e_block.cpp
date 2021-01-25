#ifndef MWORLD_SIMULATION_BLOCKS_E_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_E_BLOCKS_H

#include <block.cpp>

class Block;
class IBlock;
class EBlock;
class ZBlock;
class MBlock;

class EBlock : public Block {

public:
    real k;
    EBlock(Matrix position, Quaternion initial_orientation, real k) : Block(position, initial_orientation, 10.0f) {
        this->k = k;
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