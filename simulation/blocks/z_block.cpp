#include <block.hpp>
#include <z_block.hpp>

namespace blocks {

    void ZBlock::React(MBlock *block, real squared_dist, const Matrix& to_cube) {
        // neutral
    };

    void ZBlock::React(IBlock *block, real squared_dist, const Matrix& to_cube) {
        // neutral
    };

    void ZBlock::React(ZBlock *block, real squared_dist, const Matrix& to_cube) {
        // Attracted
        // Attraction between zblocks is the strongest out of all blocks
        auto &force = to_cube;
        AddTorque(force, this->position, Block::force_dt / squared_dist * 1 * 1.5); // as distance increases, force also decreases
    };

    void ZBlock::React(EBlock *block, real squared_dist, const Matrix& to_cube) {
        // neutral
    };

}
