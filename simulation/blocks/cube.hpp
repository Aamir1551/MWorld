#ifndef MWORLD_SIMULATION_BLOCKS_CUBE_H
#define MWORLD_SIMULATION_BLOCKS_CUBE_H

#include <iostream>
#include <vector>
#include <omp.h>

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

        // Primary
        Matrix position = Matrix(3.0, 1.0); // Position of the cube
        Quaternion orientation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);  // Cube Orientation

        // Cube Momentum's
        Matrix momentum = Matrix(3, 1);
        Matrix angular_momentum = Matrix(3, 1);

        // Secondary
        Matrix linear_velocity = Matrix(3, 1);
        Matrix angular_velocity = Matrix(3, 1);

        // constants
        real const kInverseInertia;
        real const kInverseMass;
        real const kCubeLength;

        /**
         * @brief Construct a new Cube object.
         *
         * @param cube_length Length of cube
         * @param position Initial position of the cube.
         * @param initial_orientation Initial orientation of cube
         * @param inverse_mass Inverse mass of cube
         * @param inverse_inertia Inverse inertia of cube
         */
        Cube(real cube_length, const Matrix &position, const Quaternion& initial_orientation = Quaternion(1, 0, 0, 0), real inverse_mass = 1.0f, real inverse_inertia = 1.0f) :
                kInverseInertia(inverse_inertia), kInverseMass(inverse_mass), kCubeLength(cube_length)
        {
            this->position = position;
            this->orientation = initial_orientation;
        };

        /**
         * @brief Updates the state of the cube
         *
         */
        void Update(real min_x, real max_x, real min_y, real max_y, real min_z, real max_z, real dt);


        /**
         * @brief Adds a torque to the cube.
         *
         * @param force The force vector in world coordinates
         * @param force_world_cooridinates The coordinates of the force in world coordinates
         * @param dt The amount of time the force was applied for
         */
        void AddTorque(Matrix const &force, Matrix const &force_world_cooridinates, real const dt);

        /**
         * @brief Applies a force to the cube. Only the linear motion of the Force is applied on the block
         *
         * @param force The force applied to the cube
         * @param dt Duration force is applied for
         */
        void AddLinearForce(Matrix const &force, real dt);

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

        /**
         * @brief Detect if there is a collision between Block c1 and Block c2. If a collision exists
         * then generate a contact and add to contact_list
         *
         * @param c1 Block in collision
         * @param c2 Block in collision
         * @param contact_list if collision exists, contact is added to list
         */
        static void CollisionDetect(Block *c1, Block *c2, vector<Contact> &contact_list);

        /**
         * @brief Check if there is a collision between the Cube and the world boundary.
         * If a collision exists, then resolve the collision, by bouncing the Cube in the opposite
         * direction of the world boundary.
         */
        static void CollisionBoundary(Cube *c1, real min_boundary_x, real max_boundary_x,
                                            real min_boundary_y, real max_boundary_y ,
                                            real min_boundary_z, real max_boundary_z);

        /**
         * @brief Resolves the contact given
         */
        void static CollisionResolution(Contact &contact);


        void static CollisionDetectAndResolve(Block *c1, Block *c2, vector<omp_lock_t>);

        virtual ~Cube() = default;
    };
};
#endif