#include <block.cpp>

class IBlock : public Block {

public:
    bool state; //state = true, means its a block+
    IBlock(Matrix position, Quaternion initial_orientation, bool state) : Block(position, initial_orientation, 10) {
        this->state = state;
    }

};