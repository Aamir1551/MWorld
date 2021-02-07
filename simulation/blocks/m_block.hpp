#ifndef MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_M_BLOCKS_H

#include <block.hpp>

namespace blocks {

    class MBlock : public Block {

    public:
        static real threshold;

        MBlock(Matrix position, Quaternion initial_orientation, real cube_length = 4.0f) : Block(position, initial_orientation, 0, cube_length) {};


        bool React(ForceOctree * tree, real delta_time) override;
        real ExtractFlareFromBlock(real deltatime) override;
        void AddFlareToBlock(real flare_amount) override;
        void Decay(real delta_time) override;
    };

}

#endif