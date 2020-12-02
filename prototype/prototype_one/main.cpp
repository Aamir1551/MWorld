#include "cube.cpp"
#include <iostream>

using namespace std;
int main()
{
    cout << "heyyyy " << endl;
    Cube c;
    real coordinates[] = {3, 2, 1};
    real force_coordinates[] = {0, 0, 1};

    Matrix world_coordinates = Matrix(3, 1, coordinates);
    Matrix force = Matrix(3, 1, force_coordinates);
    c.add_torque(force, world_coordinates, 0.5);
    c.GetOrientationMatrix().print();
}