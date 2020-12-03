#include <matrix.h>
#include <quaternion.hpp>
#include <iostream>
class Cube
{
public:
    //primary
    Matrix position = Matrix(3, 1, 0);
    Quaternion orientation = Quaternion(1, 0, 0, 0);

    Matrix momentum = Matrix(3, 1, 0);
    Matrix angular_momentum = Matrix(3, 1, 0);

    //secondary
    Quaternion spin = Quaternion(0, 0, 0, 0);
    Matrix velocity = Matrix(3, 1, 0);
    Matrix angular_velocity = Matrix(3, 1, 0);

    //constants
    float inertia = 1.0f;
    float inverse_inertia = 1.0f;
    float mass = 1.0f;
    float inverse_mass = 0.0f;

    Cube(Matrix position)
    {
        this->position = position;
    };

    void Update()
    {
        velocity = momentum * inverse_mass;
        angular_velocity = angular_momentum * inverse_inertia;
        Quaternion q(0, angular_velocity(0, 0), angular_velocity(1, 0), angular_velocity(2, 0));
        Quaternion spin = q * orientation * 0.5f;
        orientation += spin;
        orientation.Normalise();
    }

    void add_torque(Matrix force, Matrix world_coordinates, float dt)
    {
        //angular_momentum += Matrix::vectorProduct(world_coordinates, force - position) * dt;
        angular_momentum += Matrix::vectorProduct(world_coordinates - this->position, force) * dt;
    }

    Matrix &GetOrientationMatrix()
    {
        return Quaternion::GetMatrixTransformation(this->orientation);
    }
};