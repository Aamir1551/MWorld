#include <block.hpp>
#include <z_block.hpp>
#include "m_block.hpp"

namespace blocks {

    void ZBlock::React(MBlock *block, real squared_dist, const Matrix& to_cube) {
        // ZBlocks are attracted to M+ blocks
        if(block->flare_value > MBlock::threshold) {
            auto &force = to_cube;
            AddLinearForce(force, Block::force_dt / squared_dist * 1 );
        }
    };

    void ZBlock::React(IBlock *block, real squared_dist, const Matrix& to_cube) {
        // neutral
    };

    void ZBlock::React(ZBlock *block, real squared_dist, const Matrix& to_cube) {
        // Zblock is attracted to ZBlock
        // Attraction between zblocks is the strongest out of all blocks
        auto &force = to_cube;
        AddLinearForce(force, Block::force_dt / squared_dist * 1 );
    };

    void ZBlock::React(EBlock *block, real squared_dist, const Matrix& to_cube) {
        // neutral
    };


    real ZBlock::ExtractFlareFromBlock() {
        return 0.0;
    };

    void ZBlock::AddFlareToBlock(real flare_amount) {};


}
