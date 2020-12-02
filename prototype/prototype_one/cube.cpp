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
    Matrix id = Matrix(3, 1, 0);
    Matrix &angular_velocity = id;

    //constants
    float inertia;
    float inverse_inertia = 1.0f;
    float mass = 0.0f;
    float inverse_mass = 0.0f;

    Cube(Matrix position)
    {
        this->position = position;
    };

    void Update(float dt)
    {
        velocity = momentum * inverse_mass;
        angular_velocity = angular_momentum * inverse_inertia;
        Quaternion q(0, angular_velocity(0, 0), angular_velocity(1, 0), angular_velocity(2, 0));
        //std::cout << "Q is : " << q.print() << std::endl;
        Quaternion spin = q * orientation * 0.5f;
        orientation += spin * dt;
        orientation.Normalise();
    }

    void add_torque(Matrix force, Matrix world_coordinates, float dt)
    {
        angular_momentum += Matrix::vectorProduct(world_coordinates, force - position);
        Update(dt);
    }

    Matrix &GetOrientationMatrix()
    {
        /*std::cout << this->orientation.r << std::endl;
        std::cout << this->orientation.i << std::endl;
        std::cout << this->orientation.j << std::endl;
        std::cout << this->orientation.k << std::endl;*/
        return Quaternion::GetMatrixTransformation(this->orientation);
    }
};