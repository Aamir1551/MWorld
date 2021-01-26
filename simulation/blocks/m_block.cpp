#include <block.hpp>
#include <m_block.hpp>

namespace blocks {

        void MBlock::React(IBlock *block) {

        };

        void MBlock::React(MBlock *block) {

        };

        void MBlock::React(ZBlock *block) {

        };

        void MBlock::React(EBlock *block) {

        };

        real MBlock::threshold = 5.0f;
        real MBlock::flare_capacity = 5.0f;
};
