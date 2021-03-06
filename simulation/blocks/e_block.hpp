#ifndef MWORLD_SIMULATION_BLOCKS_E_BLOCKS_H
#define MWORLD_SIMULATION_BLOCKS_E_BLOCKS_H

#include <matrix.hpp>
#include <settings.hpp>
#include <block.hpp>

namespace blocks
{
    class EBlock : public Block {

    public:
        real k;
        real static capacity;

        EBlock(const Matrix& position, const Quaternion& initial_orientation, bool state, real cube_length = 4.0f) : Block(position, initial_orientation, 0.0f, cube_length) {
            if(state == true) {
                this->k = 2;
            } else {
                this->k = 0.5;
            }
            this->block_type=3;
        };

        bool ReactBarnesHut(ForceOctree * tree, real delta_time) override;

        real ExtractFlareFromBlock(real delta_time) override;
        void AddFlareToBlock(real flare_amount, Block *b) override;
        void UpdateFlare() override;
        void ReactSerial(EBlock *b, real delta_time) override;
        ~EBlock() override = default;

    };


} // namespace blocks
#endif