#include <block.hpp>
#include <z_block.hpp>

namespace blocks {

    void ZBlock::React(MBlock *block, real dist, const Matrix& to_cube) {
        // neautral
    };

    void ZBlock::React(IBlock *block, real dist, const Matrix& to_cube) {
        // neautral
    };

    void ZBlock::React(ZBlock *block, real dist, const Matrix& to_cube) {
        // neautral
        auto &force = to_cube;
        AddTorque(force, this->position, Block::force_dt / dist / dist * 1 * 1.1); // as distance increases, force also decreases

    };

    void ZBlock::React(EBlock *block, real dist, const Matrix& to_cube) {
        // neautral
    };

}
