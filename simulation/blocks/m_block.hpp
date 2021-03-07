#ifndef MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H

#include <block.hpp>

namespace blocks {

    class MBlock : public Block {

    public:
        static real threshold;
        static real capacity;

        MBlock(const Matrix& position, const Quaternion& initial_orientation, real cube_length = 4.0f) : Block(position, initial_orientation, 0, cube_length) {
            this->block_type = 2;
        };

        bool ReactBarnesHut(ForceOctree * tree, real delta_time) override;
        real ExtractFlareFromBlock(real deltatime) override;
        void AddFlareToBlock(real flare_amount, Block *b) override;
        void Decay(real delta_time) override;
        void UpdateFlare() override;
        void ReactSerial(ZBlock *b, real delta_time) override;

        ~MBlock() override =default;
    };

}

#endif