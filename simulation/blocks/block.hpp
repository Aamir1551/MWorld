#ifndef MWORLD_SIMULATION_BLOCK_H
#define MWORLD_SIMULATION_BLOCK_H

#include <cube.hpp>

namespace blocks {

    class IBlock;
    class EBlock;
    class MBlock;
    class ZBlock;
    class Cube;

    class Block : public Cube {

    public:

        real flare_value = 0.0f;
        real flare_inc = 0.0f;
        real static force_dt;
        real static flare_capacity;

        Block(Matrix position, Quaternion initial_orientation, real initial_flare_amount, real cube_length = 4.0f) : Cube(cube_length, position,
                                                                                                 initial_orientation,
                                                                                                 1.0f,
                                                                                                 1.0f) {
            this->flare_value = initial_flare_amount;

        };

        void SetLinearMomentum(Matrix &linear_momentum);

        void SetAngularMomentum(Matrix &angular_momentum);

        //when another block is closest to you

        virtual void React(IBlock * block, real squared_dist, const Matrix& to_cube, real deltatime) = 0;

        virtual void React(MBlock * block, real squared_dist, const Matrix& to_cube, real deltatime) = 0;

        virtual void React(ZBlock * block, real squared_dist, const Matrix& to_cube, real deltatime) = 0;

        virtual void React(EBlock * block, real squared_dist, const Matrix& to_cube, real deltatime) = 0;

        virtual real ExtractFlareFromBlock(real deltatime) = 0;

        virtual void AddFlareToBlock(real flare_amount) = 0;

        virtual void spin(Matrix const &force_direction) {
            this->angular_momentum = force_direction * this->flare_value * 0.00001  ;
        }

        void UpdateFlare() {
            this->flare_value = std::min(this->flare_inc + this->flare_value, Block::flare_capacity);
        };
    };

}
#endif