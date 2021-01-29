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
        static real flare_capacity;

        EBlock(Matrix position, Quaternion initial_orientation, real k) : Block(position, initial_orientation, 0.0f) {
            this->k = k;
        };

        void React(IBlock * block, real dist, const Matrix &to_cube) override;

        void React(MBlock * block, real dist, const Matrix &to_cube) override;

        void React(ZBlock * block, real dist, const Matrix &to_cube) override;

        void React(EBlock * block, real dist, const Matrix &to_cube) override;

        real ExtractFlareFromBlock() override;

        void AddFlareToBlock(real flare_amount) override;

    };


} // namespace blocks
#endif