#include <iostream>
#include "../linearAlg/src/matrix.h"
#include "../catch/catch.hpp"

int main()
{

    //float *arr = (float *)malloc(4);
    cout << "This is running" << endl;
    float arr[4] = {1, 2, 3, 4};

    Matrix a = Matrix(2, 2, arr);

    Matrix b = Matrix::createMatrixArange(3, 3);
    b.print();
    Matrix &c = a * b;
    cout << a.get_num_cols() << a.get_num_rows() << endl;
    cout << b.get_num_cols() << b.get_num_rows() << endl;
    cout << Matrix::same_shape(a, b) << endl;
    if (!Matrix::same_shape(a, b))
    {
        cout << "sadf" << endl;
    }
    c.print();
}