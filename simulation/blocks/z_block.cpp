#include <block.hpp>
#include <z_block.hpp>
#include "m_block.hpp"

namespace blocks {

    void ZBlock::React(MBlock *block, real squared_dist, const Matrix& to_cube, real deltimatime) {
        // ZBlocks are attracted to M+ blocks
        if(block->flare_value > MBlock::threshold) {
            auto &force = to_cube;
            AddLinearForce(force, Block::force_dt / squared_dist * 1 );
        }
    };

    void ZBlock::React(IBlock *block, real squared_dist, const Matrix& to_cube, real deltimatime) {
        // neutral
    };

    void ZBlock::React(ZBlock *block, real squared_dist, const Matrix& to_cube, real deltimatime) {
        // Zblock is attracted to ZBlock
        // Attraction between zblocks is the strongest out of all blocks
        auto &force = to_cube;
        AddLinearForce(force, Block::force_dt / squared_dist * 1 );
    };

    void ZBlock::React(EBlock *block, real squared_dist, const Matrix& to_cube, real deltimatime) {
        // neutral
    };


    real ZBlock::ExtractFlareFromBlock(real deltimatime) {
        return 0.0;
    };

    void ZBlock::AddFlareToBlock(real flare_amount) {};
}
