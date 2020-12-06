#ifndef MWORLD_NUMERICS_LINEAR_ALG_MATRIX_H
#define MWORLD_NUMERICS_LINEAR_ALG_MATRIX_H

#if defined(__GNUC__) || defined(__ICL) || defined(__clang__)
#define EXPECT(x, y) (__builtin_expect((x), (y)))
#else
#define EXPECT(x, y) (x)
#endif

#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <string>
#include <utility>

#include <settings.hpp>

namespace numerics
{
    class Matrix
    {
    protected:
        settings::real *values;
        // each of the "cols" number of contiguous "rows" number of elements in values represents the cols row in the matrix.
        int rows;
        int cols;
        std::string static GenerateError(std::string operation, Matrix const &a, Matrix const &b);
        std::string static GenerateError(std::string operation, Matrix const &a);

    public:
        //try experimenting with using & instead of pointers
        explicit Matrix(int rows, int cols);
        explicit Matrix(int rows, int cols, int factor);
        explicit Matrix(int rows, int cols, settings::real *values);
        Matrix(const Matrix &a); //copy constructor -- only used when object is being created

        std::pair<int, int> shape()
        {
            return std::make_pair(this->rows, this->cols);
        }

        void print_shape()
        {
            std::cout << "(" << this->rows << "," << this->cols << ")" << std::endl;
        }

        Matrix &operator=(const Matrix &a); //assignment operator

        Matrix static &Reshape(Matrix a, int new_row, int new_cols);
        Matrix static *CreateMatrixArange(int n, int m);

        settings::real inline const *const GetValues()
        {
            return values;
        };

        settings::real inline operator()(int row, int col) const
        {
            if (EXPECT(row < this->rows && col < this->cols, true))
            {
                return this->values[row * this->cols + col];
            }
            else
            {
                throw std::invalid_argument(
                    GenerateError("Invalid index", *this));
            }
        };

        bool operator==(Matrix const &a) const;
        bool static inline IsSameShape(Matrix const &a, Matrix const &b)
        {
            return a.rows == b.rows && a.cols == b.cols;
        }; //get meaning of inlines -- inline functions need to be defined in the header

        virtual settings::real Norm(Matrix const &a);        //returns the euclidean norm of a matrix
        virtual settings::real SquaredNorm(Matrix const &a); //returns the euclidean norm of a matrix

        Matrix static &MatMul(Matrix const &a, Matrix const &b); // returns the matrix if success, otherwise it throws an error or if an element is nan, it returns 1
        virtual int MatMul(Matrix const &a);                     // returns 0 if success, otherwise it throws an error or if an element is nan, it returns 1
        virtual int Inv();                                       // returns 0 if success, otherwise it throws an error or if an element is nan, it returns 1
        virtual int Determinent() const;

        int Transpose(); // returns 0 if success
        Matrix static &Transpose(Matrix const &a);

        Matrix &operator+(Matrix const &a) const;
        Matrix &operator-(Matrix const &a) const;
        Matrix &operator*(Matrix const &a) const;
        Matrix &operator/(Matrix const &a) const;

        Matrix static &VectorProduct(Matrix const &a, Matrix const &b);
        settings::real static Dot(Matrix const &a, Matrix const &b);

        Matrix &operator+=(Matrix const &a);
        Matrix &operator-=(Matrix const &a);
        Matrix &operator*=(Matrix const &a);
        Matrix &operator/=(Matrix const &a);
        //add serialize function

        Matrix &operator+=(settings::real a);
        Matrix &operator-=(settings::real a);
        Matrix &operator*=(settings::real a);
        Matrix &operator/=(settings::real a);

        Matrix &operator+(settings::real a) const;
        Matrix &operator-(settings::real a) const;
        Matrix &operator*(settings::real a) const;
        Matrix &operator/(settings::real a) const;

        void static print(Matrix const &a);
        void print() const;
        ~Matrix() { delete[](values); }; // not too sure whether this is done correctly
    };
    //cpplint.exe --filter=-legal/copyright,-whitespace/braces,-whitespace/newline matrix.cpp
} // namespace numerics
#endif