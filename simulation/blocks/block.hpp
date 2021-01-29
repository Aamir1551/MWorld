#ifndef MWORLD_SIMULATION_BLOCK_H
#define MWORLD_SIMULATION_BLOCK_H

#include <cube.hpp>

namespace blocks {

    class IBlock;
    class EBlock;
    class MBlock;
    class ZBlock;
    class Cube;

    class Block : public Cube { //will have to create a header file for cube

    public:

        real flare_value = 0.0f;
        real flare_inc = 0.0f;
        real static force_dt;

        Block(Matrix position, Quaternion initial_orientation, real initial_flare_amount) : Cube(4.0f, position,
                                                                                                 initial_orientation,
                                                                                                 1.0f,
                                                                                                 1.0f) {
            this->flare_value = initial_flare_amount;

        };

        void SetLinearMomentum(Matrix &linear_momentum);

        void SetAngularMomentum(Matrix &angular_momentum);

        //when another block is closest to you

        virtual void React(IBlock * block, real squared_dist, const Matrix& to_cube) = 0;

        virtual void React(MBlock * block, real squared_dist, const Matrix& to_cube) = 0;

        virtual void React(ZBlock * block, real squared_dist, const Matrix& to_cube) = 0;

        virtual void React(EBlock * block, real squared_dist ,const Matrix& to_cube) = 0;

        virtual real ExtractFlareFromBlock() = 0;

        virtual void AddFlareToBlock(real flare_amount) = 0;

        void spin(Matrix &force_direction, Matrix &force_relative_coordinates) {
             Cube::AddTorque(force_direction * this->flare_value, this->position + force_relative_coordinates, 0.01);
        }

        virtual void UpdateFlare() {
            this->flare_value += this->flare_inc;
        };

        void DecayFlare() {
            this->flare_value *= 0.1;
        }

        void RotateBlock() {
            this->angular_velocity += this->flare_inc; // or something like this???
        }

    };

}
#endif