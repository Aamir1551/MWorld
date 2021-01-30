#ifndef MWorld_SIMULATION_BLOCKS_I_BLOCKS
#define MWorld_SIMULATION_BLOCKS_I_BLOCKS

#include <block.hpp>


namespace blocks {
    class IBlock : public Block {

    public:
        bool state; //state = true, means its a I+ block
        float a;
        float b;
        static real threshold;


        IBlock(Matrix position, Quaternion initial_orientation, bool state) : Block(position, initial_orientation, 0) {
            this->state = state;
            this->a = 1.0f * (real) this->state;
            this->b = (real) (1 - this->state);
        }

        void React(IBlock * block, real dist, const Matrix& to_cube) override;
        void React(MBlock * block, real dist, const Matrix& to_cube) override;
        void React(ZBlock * block, real dist, const Matrix& to_cube) override;
        void React(EBlock * block, real dist, const Matrix& to_cube) override;

        real ExtractFlareFromBlock() override {
            return this->a + this->b * (real) (this->flare_value < IBlock::threshold);
        };

        void spin(Matrix const &force_direction) override {
            this->angular_momentum =  force_direction * (this->a + this->b * (real) (this->flare_value < IBlock::threshold)) * 0.0001;
        }

        void AddFlareToBlock(real flare_amount) override {
            this->flare_inc += flare_amount * this->b;
        };

    };


}
#endif