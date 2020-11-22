#ifndef MATRIX3_H
#define MATRIX3_H
//matrix3 is a 3x3 matrix
//matrix3 is a faster option to use instead of matrix for multiplication/manipulation/operations/inverse on 3x3 matrices

#include "matrix.h"
class Matrix3 : public Matrix
{
public:
    Matrix3();

    real norm(Matrix const &a);        //returns the euclidean norm of a matrix
    real squaredNorm(Matrix const &a); //returns the euclidean norm of a matrix

    Matrix static &MatMul(Matrix const &a, Matrix const &b); // returns the matrix if success, otherwise it throws an error or if an element is nan, it returns 1
    int MatMul(Matrix const &a);                             // returns 0 if success, otherwise it throws an error or if an element is nan, it returns 1
    int inv();                                               // returns 0 if success, otherwise it throws an error or if an element is nan, it returns 1
    int determinant() const;
    ~Matrix3() { free(values); };
};

#endif