#ifndef MWORLD_SIMULATION_BLOCKS_Z_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_Z_BLOCKS_H

#include <block.hpp>
#include <octree.hpp>
#include <force_octree.hpp>

namespace blocks {
    class ZBlock : public Block {

    public:
        ZBlock(Matrix position, Quaternion initial_orientation, real cube_length = 4.0f) : Block(position, initial_orientation, 0.0f, cube_length) {
            this->block_type = 1;
        }

        bool React(ForceOctree * tree, real delta_time) override;

        real ExtractFlareFromBlock(real deltimatime) override;
        void AddFlareToBlock(real flare_amount, Block *b) override;
        void UpdateFlare() override;
        void Decay(real delta_time) override;
        ~ZBlock() override = default;
    };
}

#endif