#include <iostream>

#include <quaternion.hpp>

using namespace std;
int main()
{
    Quaternion a(7, 0, 0, 0);
    Quaternion b(7, 0, 0, 0);

    //Quaternion c = a + b;
    //cout << a.Magnitude() << endl;
    //TODO
    //make sure all inline functions have a defintion in their header files. that is a rule in cpp
    cout << a.Magnitude() << endl;
    return 0;
}