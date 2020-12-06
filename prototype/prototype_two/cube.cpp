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
    Matrix velocity = Matrix(3, 1);
    Matrix angular_velocity = Matrix(3, 1);

    //constants
    real const inertia = 1.0f;
    real const inverse_inertia = 1.0f;
    real const mass = 1.0f;
    real const inverse_mass = 0.0f;

    /**
     * @brief Construct a new Cube object.
     * 
     * @param position Initial position of the cube.
     * @param initial_orientation Initial orientation of cube
     * @param mass Mass of cube
     * @param inverse_mass Inverse mass of cube
     * @param inertia Intertia of cube
     * @param inverse_inertia Inverse intertia of cube
     */
    Cube(Matrix position, Quaternion initial_orientation = Quaternion(1, 0, 0, 0), real mass = 1.0f,
         real inverse_mass = 1.0f, real inertia = 1.0f, real inverse_inertia = 1.0f) : mass(mass), inverse_mass(inverse_mass),
                                                                                       inertia(inertia), inverse_inertia(inverse_inertia)
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
        //velocity = momentum * inverse_mass;
        angular_velocity = angular_momentum * inverse_inertia;
        Quaternion q(0, angular_velocity(0, 0), angular_velocity(1, 0), angular_velocity(2, 0));
        Quaternion spin = q * orientation * 0.5f;
        orientation += spin;
        orientation.Normalise();
    }

    /**
     * @brief Returns the equavilent vector of a given vector in cube coordinates
     * 
     * @param world_vector 
     * @return Matrix 
     */
    Matrix ConvertToCubeCoordinates(Matrix const world_vector)
    {
        real cube_coordinates_values4[] = {world_vector(0, 0), world_vector(1, 0), world_vector(2, 0), 0};
        Matrix cube_coordintes_4(4, 1, cube_coordinates_values4);
        Matrix cube_coordinates_4 = Matrix::MatMul(this->GetOrientationMatrix(), cube_coordintes_4);
        real cube_coordinates_values[] = {cube_coordinates_4(0, 0), cube_coordinates_4(1, 0), cube_coordinates_4(2, 0)};
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
        Matrix force_cube_coordinates = ConvertToCubeCoordinates(force);
        Matrix r = ConvertToCubeCoordinates(force_world_cooridinates - this->position);
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
     * @brief Gets the Orientation Matrix of the Cube Object
     * 
     * @return Matrix& 
     */
    Matrix &GetOrientationMatrix()
    {
        return Quaternion::GetMatrixTransformation(this->orientation);
    }
};