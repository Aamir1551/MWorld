#ifndef MWORLD_SIMULATION_BLOCKS_CUBE_H
#define MWORLD_SIMULATION_BLOCKS_CUBE_H

#include <iostream>
#include <vector>

#include <matrix.hpp>
#include <quaternion.hpp>

using namespace numerics;
using namespace settings;
using namespace std;

namespace blocks {

    class Block;
    struct Contact
    {
        numerics::Matrix point;     //collision point
        settings::real penetration; //The amount of penetration
        Matrix normal;         // The contact normal // =-1
        Block *body1;                //The body pointer of the first cube //=nullptr
        Block *body2;                //The body pointer of the second cube //=nullptr
    };

    class Cube {
    public:

        //primary
        Matrix position = Matrix(3.0, 1.0);
        Quaternion orientation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

        Matrix momentum = Matrix(3, 1);
        Matrix angular_momentum = Matrix(3, 1);

        //secondary
        Matrix linear_velocity = Matrix(3, 1);
        Matrix angular_velocity = Matrix(3, 1);

        //constants
        real const inverse_inertia;
        real const inverse_mass;
        real const cube_length;

        /**
         * @brief Construct a new Cube object.
         *
         * @param cube_length Length of cube
         * @param position Initial position of the cube.
         * @param initial_orientation Initial orientation of cube
         * @param inverse_mass Inverse mass of cube
         * @param inverse_inertia Inverse inertia of cube
         */
        Cube(real cube_length, Matrix position, Quaternion initial_orientation = Quaternion(1, 0, 0, 0),
             real inverse_mass = 1.0f, real inverse_inertia = 1.0f) : cube_length(cube_length),
                                                                      inverse_mass(inverse_mass),
                                                                      inverse_inertia(inverse_inertia) {
            this->position = position;
            this->orientation = initial_orientation;
        };

        /**
         * @brief Updates the state of the cube
         *
         */
        void Update();


        /**
         * @brief Adds a torque to the cube.
         *
         * @param force The force vector in world coordinates
         * @param force_world_cooridinates The coordinates of the force in world coordinates
         * @param dt The amount of time the force was applied for
         */
        void AddTorque(Matrix const &force, Matrix const &force_world_cooridinates, real const dt);

        void AddLinearForce(Matrix const &force_direction, real dt);

        /**
         * @brief Set the Angular Momentum To Zero
         *
         */
        void SetAngularMomentumToZero();


        /**
         * @brief Set the Linear Momentum To Zero
         *
         */
        void SetLinearMomentumToZero();

        /**
         * @brief Gets the Orientation Matrix of the Cube Object. (Can be used to transform a vector from
         * cube coordinates to world coordinates -? not too sure )
         *
         * @return Matrix&
         */
        Matrix GetOrientationMatrix() const;

        static void CollisionDetect(Block *c1, Block *c2, vector<Contact> &contact_list);
        void static CollisionResolution(Contact &contact);
    };
};
#endif