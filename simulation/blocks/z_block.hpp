#ifndef MWORLD_SIMULATION_BLOCKS_Z_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_Z_BLOCKS_H

#include <block.hpp>
#include <collision_octree.hpp>
#include <force_octree.hpp>

namespace blocks {
    class ZBlock : public Block {

    public:
        static real threshold;
        ZBlock(const Matrix& position, const Quaternion& initial_orientation, real cube_length = 4.0f) : Block(position, initial_orientation, 0.0f, cube_length) {
            this->block_type = 1;
        }

        bool ReactBarnesHut(ForceOctree * tree, real delta_time) override;

        real ExtractFlareFromBlock(real delta_time) override;
        void AddFlareToBlock(real flare_amount, Block *b) override;
        void UpdateFlare() override;
        void Decay(real delta_time) override;
        void ReactSerial(ZBlock *b, real delta_time) override;
        void ReactSerial(MBlock *b, real delta_time) override;

        ~ZBlock() override = default;
    };
}

#endif