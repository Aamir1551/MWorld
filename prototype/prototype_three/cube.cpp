#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <matrix.hpp>
#include <quaternion.hpp>

using namespace numerics;
using namespace settings;

// TODO
// 1) Make an GetTransformMatrix method (in Matrix class) but returns 3x3 Matrix && the same for inverse as well
// 2) Add Edge to Edge collision pg319 in pdf and pg296 in book

class Cube;

struct Contact
{
    numerics::Matrix point;
    real penetration;
    int contact_normal = -1;
    Cube *body1 = nullptr;
    Cube *body2 = nullptr;
    bool is_contact;
};

class Cube
{
public:
    //primary
    Matrix position = Matrix(3.0, 1.0);
    Quaternion orientation = Quaternion(1.0f, 0.0f, 0.0f, 0.0f);

    Matrix momentum = Matrix(3, 1);
    Matrix angular_momentum = Matrix(3, 1);

    //secondary
    Quaternion spin = Quaternion(0, 0, 0, 0);
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
     * @param inverse_inertia Inverse intertia of cube
     */
    Cube(real cube_length, Matrix position, Quaternion initial_orientation = Quaternion(1, 0, 0, 0),
         real inverse_mass = 1.0f, real inverse_inertia = 1.0f) : cube_length(cube_length),
                                                                  inverse_mass(inverse_mass),
                                                                  inverse_inertia(inverse_inertia)
    {
        this->position = position;
        this->orientation = initial_orientation;
    };

    /**
     * @brief Updates the state of the cube
     * 
     */
    void Update()
    {

        /*
        Equations of motion being used are:
            p = Linear momentum
            L = Angular momentum

            v = Linear velocity
            w = Angular velocity
            
            m = Linear Intertia AKA mass
            I = Angular Intertia AKA moment of intertia

            x = Linear position
            θ = Angular position   

            v = p * 1/m
            w = L * 1/I

            dx/dt = v
            dθ/dt = w
        */

        /*
        Updating orientation quaternion involves the steps:
            1)  Convert Scaled axes representation of angular velocity to a quaternion 
                scaled axes quaternion = (angular_velocity[0], angular_velocity[1], angular_velocity[2])
            2)  Use the equation: 
                        new orientation = old orientation + 1/2 * scaled axes quaternion * old orientation
        
        */

        linear_velocity = momentum * inverse_mass;
        angular_velocity = angular_momentum * inverse_inertia;

        Quaternion q(0, angular_velocity(0, 0), angular_velocity(1, 0), angular_velocity(2, 0));
        Quaternion spin = q * orientation;

        position += linear_velocity;
        orientation += spin;
        orientation.Normalise();
    }

    /**
     * @brief Returns the equavilent vector of a given vector in cube coordinates
     * 
     * @param world_vector 
     * @return Matrix 
     */
    Matrix ConvertToCubeCoordinates(Matrix const world_vector) const
    {
        real world_coordinates_values[] = {world_vector(0, 0), world_vector(1, 0), world_vector(2, 0), 0};
        Matrix world_coordintes_vector(4, 1, world_coordinates_values);
        Matrix cube_coordinates_vector = Matrix::MatMul((this->GetOrientationMatrix()), world_coordintes_vector);
        real cube_coordinates_values[] = {cube_coordinates_vector(0, 0), cube_coordinates_vector(1, 0), cube_coordinates_vector(2, 0)};
        return Matrix(3, 1, cube_coordinates_values);
    }

    /**
     * @brief Adds a torque to the cube. 
     * 
     * @param force The force vector in world coordinates
     * @param force_world_cooridinates The coordinates of the force in world coordinates
     * @param dt The amount of time the force was applied for
     */
    void AddTorque(Matrix const force, Matrix const force_world_cooridinates, real const dt)
    {
        /*
            Initially the force and the force_world_coordinates are in world coordintes. However, the 
            angular_momentums (which is a vector quantity) of the cube is in cube_coordinates. 
            Hence, we need to first transform the force, and the force_world_coordinates to cube coordinates.
        */

        /*
        Equations of motion being used are:
            p = Linear momentum
            L = Angular momentum

            torque = perpendicular distance form pivot * force

            linear impulse (Δp) = force * time
            Angular impulse (ΔL) = torque * time

            dp/dt = Δp (Rate of change of linear momentum)
            dL/dt = ΔL (Rate of change of angular momentum)

            New linear momentum = old linear momentum + linear impulse
            New angular momentum = old angular momentum + angular impulse

       */
        Matrix force_cube_coordinates = ConvertToCubeCoordinates(force);
        Matrix r = ConvertToCubeCoordinates(force_world_cooridinates - this->position);

        // Torque is calculated via Matrix::VectorProduct(force_cube_coordinates, r) * dt

        momentum += force * dt;
        angular_momentum += Matrix::VectorProduct(force_cube_coordinates, r) * dt;
    }

    /**
     * @brief Set the Angular Momentum To Zero
     * 
     */
    void SetAngularMomentumToZero()
    {
        angular_momentum = Matrix(3, 1);
    }

    /**
     * @brief Gets the Orientation Matrix of the Cube Object. (Can be used to transform a vector from 
     * cube coordinates to world coordinates -? not too sure )
     * 
     * @return Matrix& 
     */
    Matrix &GetOrientationMatrix() const
    {
        return Quaternion::GetMatrixTransformation(this->orientation);
    }

    /**
     * @brief Get the Inverse Orientation Matrix of the cube Object.
     * 
     * @return Matrix& 
     */
    Matrix &GetInverseOrientationMatrix() const
    {
        return Quaternion::GetInverseMatrixTransformation(this->orientation);
    }

    Matrix GetVerticesWorldCoordinates() const
    {
        real vertices[24];
        real diff = cube_length / 2;
        for (unsigned int i = 0; i < 8; i++)
        {
            vertices[i + 0] = (i % 2 == 0 ? -diff : diff);
            vertices[i + 16] = (((int)std::floor(i / 2)) % 2 == 0 ? -diff : diff);
            vertices[i + 24] = (((int)std::floor(i / 4)) % 2 == 0 ? -diff : diff);
        }
        Matrix out = Matrix::MatMul(this->GetInverseOrientationMatrix(), Matrix(3, 8, vertices));
        return out;
    }

    /**
     * @brief Detect collision between Cube A and Cube B
     * 
     * @param A 
     * @param B 
     * @return Contact 
     */
    Contact static CollisionDetect(Cube &a, Cube &b)
    {
        b.GetVerticesWorldCoordinates().GetColumns();
    }

    // make the below two functions private
    /**
     * @brief Returns the contact information of a cube in contact with point
     * 
     * @param A 
     * @param point 
     * @return Contact 
     */
    static bool CollisionDetectCubePoint(Cube &a, Matrix &point, Contact &contact_info)
    {
        Matrix point_cube_coordinate = numerics::Matrix::MatMul(a.GetInverseOrientationMatrix(), point);

        real min_depth = a.cube_length - std::abs(point_cube_coordinate(0, 0));
        int normal = point_cube_coordinate(0, 0) < 0 ? 0 : 1;
        if (min_depth < 0)
            return false;

        real depth = a.cube_length - std::abs(point_cube_coordinate(1, 0));
        if (depth < 0)
            return false;
        else if (depth < min_depth)
        {
            min_depth = depth;
            normal = point_cube_coordinate(1, 0) < 0 ? 2 : 3;
        }

        real depth = a.cube_length - std::abs(point_cube_coordinate(2, 0));
        if (depth < 0)
            return false;
        else if (depth < min_depth)
        {
            min_depth = depth;
            normal = point_cube_coordinate(2, 0) < 0 ? 4 : 5;
        };

        contact_info.point = point;
        contact_info.is_contact = true;
        contact_info.penetration = min_depth;
        contact_info.contact_normal = normal;
        return 1;
    }

    Contact static CollisionDetectEdgeEdge(Matrix &edge1, Matrix &edge2)
    {
    }
};