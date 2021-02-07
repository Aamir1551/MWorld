#ifndef MWORLD_SIMULATION_BLOCK_H
#define MWORLD_SIMULATION_BLOCK_H

#include <cube.hpp>

namespace blocks {

    class IBlock;
    class EBlock;
    class MBlock;
    class ZBlock;
    class Cube;
    class Octree;
    class ForceOctree;

    class Block : public Cube {

    public:

        real flare_value = 0.0f;
        real flare_inc = 0.0f;
        real static flare_capacity; // change this for the eblock, so we can have eblocks help us make or gates
        real static theta;
        bool locked; // When a block is locked, then forces do not act on it

        Block(Matrix position, Quaternion initial_orientation, real initial_flare_amount, real cube_length = 4.0f) : Cube(cube_length, position,
                                                                                                 initial_orientation,
                                                                                                 1.0f,
                                                                                                 1.0f) {
            this->flare_value = initial_flare_amount;
            this->locked = false;

        };

        bool ApplyForceFromBlock(ForceOctree* tree, real delta_time, int cell_count, const Matrix &com, Matrix &inc_force);

        void SetLinearMomentum(Matrix &linear_momentum);

        void SetAngularMomentum(Matrix &angular_momentum);

        virtual bool React(ForceOctree * tree, real delta_time) = 0;

        virtual real ExtractFlareFromBlock(real deltatime) = 0;

        virtual void AddFlareToBlock(real flare_amount) = 0;

        virtual void spin(Matrix const &force_direction);

        virtual void UpdateFlare();

        virtual void Decay(real delta_time);

    };

}
#endif