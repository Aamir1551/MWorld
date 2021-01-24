#include <block.cpp>

class EBlock : public Block {

public:
    real k;
    EBlock(Matrix position, Quaternion initial_orientation, real k) : Block(position, initial_orientation, 10.0f) {
        this->k = k;
    }

};