#include <block.hpp>
#include <i_block.hpp>

namespace blocks {

    void IBlock::React(IBlock *block) {

    };

    void IBlock::React(MBlock *block) {

    };

    void IBlock::React(ZBlock *block) {

    };

    void IBlock::React(EBlock *block)  {


    };

    real IBlock::threshold = 2.0f;
};
