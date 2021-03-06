#ifndef MWORLD_NUMERICS_LINEAR_ALG_MATRIX_H
#define MWORLD_NUMERICS_LINEAR_ALG_MATRIX_H

#if defined(__GNUC__) || defined(__ICL) || defined(__clang__)
#define EXPECT(x, y) (__builtin_expect((x), (y)))
#else
#define EXPECT(x, y) (x)
#endif

#include <cstdlib>
#include <cstdio>

#include <iostream>
#include <string>
#include <utility>
#include <stdexcept>

#include <settings.hpp>

namespace numerics
{
    class Quaternion;
    class Matrix
    {
    protected:
        //see operator () to see how "real" number is laid out within values
        settings::real *values;

        // Number of rows in matrix
        int rows;
        // Number of columns in matrix
        int cols;

        /**
         * @brief Generates a shape error for a given operation
         * 
         * @param operation The operation that caused the error
         * @param a Matrix a of operand 
         * @param b Matrix b of operand
         * @return std::string representing the error.
         */
        std::string static GenerateError(std::string operation, Matrix const &a, Matrix const &b);

        /**
         * @brief Generates a shape error for a given operation
         * 
         * @param operation The operation that caused the error
         * @param a Matrix a of operand 
         * @return std::string representing the error.
         */
        std::string static GenerateError(std::string operation, Matrix const &a);

    public:
        /**
         * @brief Construct a new Matrix object
         * 
         * @param rows 
         * @param cols 
         */

        Matrix();
        explicit Matrix(int rows, int cols);

        /**
         * @brief Construct a new Matrix object
         * 
         * @param rows 
         * @param cols 
         * @param factor The value to set the diagonals of the matrix to.
         */
        explicit Matrix(int rows, int cols, int factor);

        /**
         * @brief Construct a new Matrix object
         * 
         * @param rows 
         * @param cols 
         * @param values The pointer to copy values from to set the initial values of the matrix.
         */
        explicit Matrix(int rows, int cols, settings::real const *values);

        /**
         * @brief Construct a new Matrix object. Is a vector of 3 values.
         *
         * @param val0
         * @param val1
         * @param val2
         */
        Matrix static CreateColumnVec(settings::real val0, settings::real val1, settings::real val2);

        /**
         * @brief Construct a new Matrix object. Is a vector of 3 values.
         *
         * @param val0
         * @param val1
         * @param val2
         * @param val3
         */
        Matrix static CreateColumnVec(settings::real val0, settings::real val1, settings::real val2, settings::real val3);


        /**
         * @brief Construct a new Matrix object. Copy constructor.
         * 
         * @param a Matrix to copy from.
         */
        Matrix(const Matrix &a);

        /**
         * @brief Construct a new Matrix object. Joins two vectors together. Both vectors are to be of shape 1xn
         * 
         * @param vec1 
         * @param vec2 
         */
        Matrix(Matrix const &vec1, Matrix const &vec2);

        /**
         * @brief Returns the shape of a matrix object.
         * 
         * @return std::pair<int, int> 
         */
        std::pair<int, int> shape() const
        {
            return std::make_pair(this->rows, this->cols);
        }

        /**
         * @brief Prints the shape of a matrix object.
         * 
         */
        void print_shape() const
        {
            std::cout << "(" << this->rows << "," << this->cols << ")" << std::endl;
        }

        /**
         * @brief Assignment operator. Copy's all values from one matrix to the next
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix &operator=(const Matrix &a);

        /**
         * @brief Reshapes a given matrix to another.
         * 
         * @param a The matrix to reshape.
         * @param new_row The new number of rows the new matrix will contain.
         * @param new_cols The new number of columns the new matrix will contain.
         * @return Matrix& 
         */
        Matrix static Reshape(Matrix a, int new_row, int new_cols);

        /**
         * @brief Create a Matrix object, where each item in the matrix is one higher than the next.
         * 
         * @param n Number of rows the matrix object will contain.
         * @param m Number of columns the matrix object will contain.
         * @return Matrix* 
         */
        Matrix static *CreateMatrixArange(int n, int m);

        /**
         * @brief Get the Values pointer of matrix.
         * 
         * @return settings::real const* const 
         */
        settings::real inline const *GetValues() const
        {
            return values;
        };

        /**
         * @brief Get the item at position (row, col) of given matrix
         * 
         * @param row 
         * @param col 
         * @return settings::real 
         */
        settings::real inline operator()(int row, int col) const
        {
            if ((row < this->rows && col < this->cols && row >= 0 && col >= 0))
            {
                return this->values[row * this->cols + col];
            }
            else
            {
                throw std::invalid_argument(
                    GenerateError("Invalid index", *this));
            }
        };

        /**
         * @brief Set the item at position (row, col) of given matrix to val
         *
         * @param row
         * @param col
         * @param val
         * @return settings::real
         */
        void inline operator()(int row, int col, settings::real val)
        {
            if ((row < this->rows && col < this->cols && row >= 0 && col >= 0))
            {
                this->values[row * this->cols + col] = val;
            }
            else
            {
                throw std::invalid_argument(
                        GenerateError("Invalid index", *this));
            }
        };

        /**
         * @brief Determines if two matrices have the same value at each index.
         * 
         * @param a 
         * @return True if all values are the same and have the same shape. Otherwise returns false.
         */
        bool operator==(Matrix const &a) const;

        /**
         * @brief Determines if two matrices have the same shape.
         * 
         * @param a 
         * @return True if matrices have same shape. Otherwise returns false.
         */
        bool static inline IsSameShape(Matrix const &a, Matrix const &b)
        {
            return a.rows == b.rows && a.cols == b.cols;
        };

        /**
         * @brief Returns the euclidean norm of a matrix
         * 
         * @param a 
         * @return settings::real 
         */
        static settings::real Norm(Matrix const &a);

        /**
         * @brief Normalises the matrix
         *
         */
        void Normalise();

        /**
         * @brief Add column vector to Matrix, along every column
         *
         */
        bool IsZero();

        void abs();

        /**
         * @brief Returns the squared euclidean norm of a matrix
         * 
         * @param a 
         * @return settings::real 
         */
        static settings::real SquaredNorm(Matrix const &a);

        /**
         * @brief Applies Matrix multiplication between two matrices. If multiplication fails, throws invalid_argument error.
         * 
         * @param a 
         * @param b 
         * @return Matrix& 
         */
        Matrix static MatMul(Matrix const &a, Matrix const &b);

        /**
         * @brief Returns the view matrix
         *
         * @param eye Position of the camera viewpoint
         * @param center Position, from which we are looking at
         * @param up Vector defining the the world Up direction
         * @return Matrix representing the view matrix
         */
        Matrix static LookAt(Matrix const &eye, Matrix const &center, Matrix const &up);


        /**
         * @brief Returns the perspective matrix
         *
         * @param fovy field of fiew y
         * @param aspect
         * @param znear The closest we can see
         * @param zfar  The furthest we can see
         * @return
         */
        Matrix static Perspective(settings::real fovy, settings::real aspect, settings::real znear, settings::real zfar);


        /**
         * @brief Converts a real number to radians
         *
         * @param degrees Angle in degrees
         * @return
         */
        settings::real static ConvertToRadians(settings::real degrees);

        /**
         * @brief Transposes a given Matrix 
         * 
         * @param a 
         * @return Matrix& 
         */
        void Transpose();

        /**
         * @brief Returns the transpose of a given matrix 
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix static Transpose(Matrix const &a);

        /**
         * @brief Elementwise Addition. 
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix operator+(Matrix const &a) const;

        /**
         * @brief Elementwise subtraction. 
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix operator-(Matrix const &a) const;

        /**
         * @brief Elementwise multiplication. 
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix operator*(Matrix const &a) const;

        /**
         * @brief Elementwise division. 
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix operator/(Matrix const &a) const;

        /**
         * @brief Calculates the vector product between 2 vectors. Vectors must have shape (3, 1).
         * 
         * @param a A marix (or vector) of shape (3, 1)
         * @param b A marix (or vector) of shape (3, 1)
         * @return Matrix& 
         */
        Matrix static VectorProduct(Matrix const &a, Matrix const &b);

        /**
         * @brief Calculates the dot product between two matrices. Matrices must have the same shape.
         * 
         * @param a A marix  
         * @param b A marix 
         * @return Matrix& 
         */
        settings::real static Dot(Matrix const &a, Matrix const &b);

        /**
         * @brief Adds a matrix elementwise to current matrix.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix &operator+=(Matrix const &a);

        /**
         * @brief Adds a matrix elementwise to current matrix.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix &operator-=(Matrix const &a);

        /**
         * @brief Subtracts a matrix elementwise from current matrix.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix &operator*=(Matrix const &a);

        /**
         * @brief Divides a matrix elementwise from current matrix.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix &operator/=(Matrix const &a);

        /**
         * @brief Adds a real number elementwise to current matrix.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix &operator+=(settings::real a);

        /**
         * @brief Subtracts a real number elementwise from current matrix.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix &operator-=(settings::real a);

        /**
         * @brief Multiplies a real number elementwise on current matrix.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix &operator*=(settings::real a);

        /**
         * @brief Divides a real number elementwise on current matrix.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix &operator/=(settings::real a);

        /**
         * @brief Returns the result of elementwise addition on current matrix with a real.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix operator+(settings::real a) const;

        /**
         * @brief Returns the result of elementwise subtraction on current matrix with a real.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix operator-(settings::real a) const;

        /**
         * @brief Returns the result of elementwise multiplication on current matrix with a real.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix operator*(settings::real a) const;

        /**
         * @brief Returns the result of elementwise division on current matrix with a real.
         * 
         * @param a 
         * @return Matrix& 
         */
        Matrix operator/(settings::real a) const;

        /**
         * @brief Prints the contents of a matrix via iostream.
         * 
         * @param a 
         */
        void static print(Matrix const &a);

        /**
         * @brief Prints matrix via iostream.
         * 
         */
        void print() const;

        /**
         * @brief Destroy the Matrix object
         * 
         */
        ~Matrix() { delete[](this->values); };

        /**
         * @brief Applies the max function at each element in the 3d vector which each of the max elements given
         *
         * @param index
         */
        void ApplyMaxVector3(settings::real max_x, settings::real max_y, settings::real max_z);

        /**
         * @brief Applies the min function at each element in the 3d vector which each of the min elements given
         *
         * @param index
         */
        void ApplyMinVector3(settings::real min_x, settings::real min_y, settings::real min_z);

        /**
         * @brief Translates the following 4x4 Matrix by translation_value
         * Translates a 4x4 matrix. For performance purposes, we will not be testing if the matrix is of size 4x4
         *
         * @param translation_matrix The amount to translate by
         */
        void Translate4by4Matrix(const Matrix& translation_matrix);

        /**
         * @brief Creates a 4x4 Matrix representing the translation value
         *
         * @param translates_matrix The amount to translate by
         */
        Matrix static CreateTranslationMatrix(const Matrix& translation_matrix);

        friend class Quaternion;

    };
} // namespace numerics
#endif