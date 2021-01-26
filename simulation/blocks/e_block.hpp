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
        EBlock(Matrix position, Quaternion initial_orientation, real k) : Block(position, initial_orientation, 10.0f) {
            this->k = k;
        };

        void React(IBlock *block) override;

        void React(MBlock *block) override;

        void React(ZBlock *block) override;

        void React(EBlock *block) override;

        real ExtractFlareFromBlock() override;

        void AddFlareToBlock(real flare_amount) override;

    };


} // namespace blocks
#endif