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

        EBlock(Matrix position, Quaternion initial_orientation, bool state) : Block(position, initial_orientation, 0.0f) {
            if(state == true) {
                this->k = 1;
            } else {
                this->k = 0.5;
            }
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