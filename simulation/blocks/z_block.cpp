#include <block.cpp>

class ZBlock : public Block {

public:
    ZBlock(Matrix position, Quaternion initial_orientation) : Block(position, initial_orientation, 10.0f) {}
};