#ifndef MWORLD_SIMULATION_BLOCKS_Z_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_Z_BLOCKS_H

#include <block.cpp>


class Block;

class IBlock;
class MBlock;
class ZBlock;
class EBlock;

class ZBlock : public Block {

public:
    ZBlock(Matrix position, Quaternion initial_orientation) : Block(position, initial_orientation, 10.0f) {}

    void React(IBlock *block) override{

    };
    void React(MBlock *block) override {

    };
    void React(ZBlock *block) override {

    };
    void React(EBlock *block) override {

    };

};

#endif