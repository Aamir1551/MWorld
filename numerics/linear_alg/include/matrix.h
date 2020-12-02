#ifndef MWORLD_NUMERICS_LINEAR_ALG_MATRIX_H
#define MWORLD_NUMERICS_LINEAR_ALG_MATRIX_H

#if defined(__GNUC__) || defined(__ICL) || defined(__clang__)
#define EXPECT(x, y) (__builtin_expect((x), (y)))
#else
#define EXPECT(x, y) (x)
#endif

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

typedef float real; //remove this. Now using settings::real

class Matrix
{
protected:
    real *values;
    // each of the "cols" number of contiguous "rows" number of elements in values represents the cols row in the matrix.
    int rows;
    int cols;
    std::string static generateError(std::string operation, Matrix const &a, Matrix const &b);
    std::string static generateError(std::string operation, Matrix const &a);

public:
    //try experimenting with using & instead of pointers
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, int factor);
    Matrix(int rows, int cols, real *values);
    Matrix(const Matrix &a);            //copy constructor -- only used when object is being created
    Matrix &operator=(const Matrix &a); //assignment operator

    Matrix static &createMatrixArange(int n, int m);

    real inline const *const getValues()
    {
        return values;
    };

    int get_num_rows()
    {
        return this->rows;
    }
    int get_num_cols()
    {
        return this->cols;
    }

    real inline operator()(int row, int col)
    {
        if (EXPECT(row < this->rows && col < this->cols, true))
        {
            return this->values[row * this->cols + col];
        }
        else
        {
            throw std::invalid_argument(
                generateError("Invalid index", *this));
        }
    };

    bool operator==(Matrix const &a) const;
    bool static inline same_shape(Matrix const &a, Matrix const &b)
    {
        return a.rows == b.rows && a.cols == b.cols;
    }; //get meaning of inlines -- inline functions need to be defined in the header

    virtual real norm(Matrix const &a);        //returns the euclidean norm of a matrix
    virtual real squaredNorm(Matrix const &a); //returns the euclidean norm of a matrix

    Matrix static &MatMul(Matrix const &a, Matrix const &b); // returns the matrix if success, otherwise it throws an error or if an element is nan, it returns 1
    virtual int MatMul(Matrix const &a);                     // returns 0 if success, otherwise it throws an error or if an element is nan, it returns 1
    virtual int inv();                                       // returns 0 if success, otherwise it throws an error or if an element is nan, it returns 1
    virtual int determinant() const;

    int transpose(); // returns 0 if success
    Matrix static &Transpose(Matrix const &a);

    Matrix &operator+(Matrix const &a) const;
    Matrix &operator-(Matrix const &a) const;
    Matrix &operator*(Matrix const &a) const;
    Matrix &operator/(Matrix const &a) const;

    Matrix static &vectorProduct(Matrix const &a, Matrix const &b);
    real static dotProduct(Matrix const &a, Matrix const &b);

    Matrix &operator+=(Matrix const &a);
    Matrix &operator-=(Matrix const &a);
    Matrix &operator*=(Matrix const &a);
    Matrix &operator/=(Matrix const &a);
    //add serialize function

    Matrix &operator+=(real a);
    Matrix &operator-=(real a);
    Matrix &operator*=(real a);
    Matrix &operator/=(real a);

    Matrix &operator+(real a) const;
    Matrix &operator-(real a) const;
    Matrix &operator*(real a) const;
    Matrix &operator/(real a) const;

    void static print(Matrix const &a);
    void print() const;
    ~Matrix() { free(values); }; // not too sure whether this is done correctly
};
//cpplint.exe --filter=-legal/copyright,-whitespace/braces,-whitespace/newline matrix.cpp
#endif