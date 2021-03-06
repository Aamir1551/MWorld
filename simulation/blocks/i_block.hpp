#ifndef MWorld_SIMULATION_BLOCKS_I_BLOCKS
#define MWorld_SIMULATION_BLOCKS_I_BLOCKS

#include <collision_octree.hpp>
#include <force_octree.hpp>
#include <block.hpp>

namespace blocks {

    class IBlock : public Block {

    public:
        bool state; //state = true, means its a I+ block
        float a;
        float b;
        static real threshold;

        IBlock(const Matrix& position, const Quaternion& initial_orientation, bool state, real cube_length = 4.0f) : Block(position, initial_orientation, 0, cube_length) {
            this->state = state;
            this->a = 1.0f * (real) this->state;
            this->b = (real) (1 - this->state);
            this->block_type = 0;
        }

        bool ReactBarnesHut(ForceOctree * tree, real delta_time) override;
        real ExtractFlareFromBlock(real delta_time) override;
        void spin(Matrix const &force_direction) override;
        void AddFlareToBlock(real flare_amount, Block *b) override;
        void Decay(real delta_time) override;
        void ReactSerial(IBlock *bl, real delta_time) override;

        ~IBlock() override = default;
    };


}
#endif