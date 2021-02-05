#ifndef MWorld_SIMULATION_BLOCKS_I_BLOCKS
#define MWorld_SIMULATION_BLOCKS_I_BLOCKS

#include <octree.hpp>
#include <force_octree.hpp>
#include <block.hpp>

namespace blocks {

    class IBlock : public Block {

    public:
        bool state; //state = true, means its a I+ block
        float a;
        float b;
        static real threshold;


        IBlock(Matrix position, Quaternion initial_orientation, bool state, real cube_length = 4.0f) : Block(position, initial_orientation, 0, cube_length) {
            this->state = state;
            this->a = 1.0f * (real) this->state;
            this->b = (real) (1 - this->state);
        }


        virtual bool React(ForceOctree * tree, real delta_time) override;

        real ExtractFlareFromBlock(real deltatime) override;
        void spin(Matrix const &force_direction) override;
        void AddFlareToBlock(real flare_amount) override;

    };


}
#endif