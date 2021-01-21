#include <iostream>

#include <matrix.hpp>
#include <quaternion.hpp>

using namespace numerics;
using namespace settings;

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

    /**
     * @brief Construct a new Cube object.
     * 
     * @param position Initial position of the cube.
     * @param initial_orientation Initial orientation of cube
     * @param inverse_mass Inverse mass of cube
     * @param inverse_inertia Inverse intertia of cube
     */
    Cube(Matrix position, Quaternion initial_orientation = Quaternion(1, 0, 0, 0),
         real inverse_mass = 1.0f, real inverse_inertia = 1.0f) : inverse_mass(inverse_mass),
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
        return Matrix::MatMul((Quaternion::GetOrientationMatrix3(this->orientation)), world_vector);
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
};