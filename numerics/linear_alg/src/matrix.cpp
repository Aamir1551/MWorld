#include <matrix.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include <settings.hpp>

using namespace std;

namespace numerics
{

    Matrix::Matrix() {
        this->cols = 0;
        this->rows = 0;
        this->values = nullptr;
    }

    Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols)
    {
        this->values = new settings::real[rows * cols];
        for (int i = 0; i < rows * cols; i++)
        {
            this->values[i] = 0;
        }
    }

    Matrix &Matrix::operator=(const Matrix &a)
    {
        if(this == &a) {
            return *this;
        }

        this->rows = a.rows;
        this->cols = a.cols;
        delete[] (this->values);
        this->values = new settings::real[rows * cols];

        for (int i = 0; i < a.rows * a.cols; i++)
        {
            this->values[i] = a.values[i];
        }
        return *this;
    }

    Matrix::Matrix(int rows, int cols, int factor)
    {
        this->rows = rows;
        this->cols = cols;
        this->values = new settings::real[rows * cols];
        for (int i = 0; i < rows * cols; i++)
        {
            this->values[i] = 0;
        }
        int t = std::min(rows, cols);
        int j = 0;
        for (int i = 0; i < t; i++)
        {
            this->values[i + cols * j] = factor;
            j++;
        }
    }

    Matrix::Matrix(Matrix const &vec1, Matrix const &vec2)
    {

        if (vec1.rows != vec2.rows || vec1.cols != 1 || vec2.cols != 1)
        {
            throw std::invalid_argument("vec1 or vec2 shapes do not conform. Shapes need to be nx1.");
        }

        this->rows = vec1.rows;
        this->cols = 2;
        this->values = new settings::real[this->rows * this->cols];

        for (int i = 0; i < this->rows; i++)
        {
            this->values[2 * i] = vec1(i, 0);
        }

        for (int i = 0; i < this->rows; i++)
        {
            this->values[2 * i + 1] = vec2(i, 0);
        }
    };

    Matrix::Matrix(const Matrix &a) //copy constructor
    {
        this->rows = a.rows;
        this->cols = a.cols;
        this->values = new settings::real[rows * cols];

        for (int i = 0; i < a.rows * a.cols; i++)
        {
            this->values[i] = a.values[i];
        }
    }

    Matrix Matrix::Reshape(Matrix const a, int new_row, int new_cols)
    {
        if (a.rows * a.cols != new_row * new_cols)
        {
            throw std::invalid_argument("Matrix Shape does not conform for Reshape");
        }
        Matrix b = Matrix(new_row, new_cols);
        for (int i = 0; i < a.rows * a.cols; i++)
        {
            b.values[i] = a.values[i];
        }
        return b;
    };

    settings::real Matrix::SquaredNorm(Matrix const &a)
    {
        settings::real sum = 0;
        for (int i = 0; i < a.rows * a.cols; i++)
        {
            sum += a.values[i] * a.values[i];
        }
        return (sum);
    };

    settings::real Matrix::Norm(Matrix const &a)
    {
        settings::real sum = 0;
        for (int i = 0; i < a.rows * a.cols; i++)
        {
            sum += a.values[i] * a.values[i];
        }
        return sqrt(sum);
    };

    std::string Matrix::GenerateError(std::string operation,
                                      Matrix const &a, Matrix const &b)
    {
        std::string msg = "Matrix shapes do not conform for " +
                          operation + ".";

        std::string a_shape = "A has shape=(" + std::to_string(a.rows) + "," +
                              std::to_string(a.cols) + ").";

        std::string b_shape = "B has shape=(" + std::to_string(b.rows) + "," +
                              std::to_string(b.cols) + ").";

        return msg + " " + a_shape + " " + b_shape;
    }

    std::string Matrix::GenerateError(std::string const operation, Matrix const &a)
    {
        std::string msg = "Matrix shapes do not conform for " + operation + ".";
        std::string a_shape = "Argument has shape=(" + std::to_string(a.rows) + "," +
                              std::to_string(a.cols) + ").";

        return msg + " " + a_shape;
    }

    void Matrix::print(Matrix const &a)
    {
        for (int i = 0; i < a.rows; i++)
        {
            for (int j = 0; j < a.cols; j++)
            {
                std::cout << a.values[i * a.cols + j] << " ";
            }
            std::cout << std::endl;
        }
    }

    Matrix *Matrix::CreateMatrixArange(int n, int m)
    {
        auto *a = new Matrix(n, m);
        for (int i = 0; i < m * n; i++)
        {
            a->values[i] = i;
        }
        return a;
    };

    Matrix::Matrix(int rows, int cols, settings::real const *const values) : rows(rows),
                                                                             cols(cols)
    {
        this->rows = rows;
        this->cols = cols;
        this->values = new settings::real[rows * cols];
        for (int i = 0; i < rows * cols; i++)
        {
            this->values[i] = values[i];
        }
    };

    void Matrix::Transpose()
    {
        auto *new_values = new settings::real[this->cols * this->rows];
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                new_values[j * this->rows + i] = this->values[i * this->cols + j];
            }
        }

        int t = this->cols;
        this->cols = this->rows;
        this->rows = t;

        delete[] values;
        this->values = new_values;
    }

    Matrix Matrix::Transpose(Matrix const &a)
    {
        auto res = Matrix(a.cols, a.rows);
        for (int i = 0; i < a.rows; i++)
        {
            for (int j = 0; j < a.cols; j++)
            {
                res.values[j * a.rows + i] = a.values[i * a.cols + j];
            }
        }
        return res;
    }

    Matrix Matrix::MatMul(Matrix const &a, Matrix const &b)
    {
        if (a.cols != b.rows)
        {
            throw std::invalid_argument(GenerateError("Multiplication", a, b));
        }

        Matrix res = Matrix(a.rows, b.cols);
        for(int i=0; i<a.rows; i++) {
            for(int j=0; j<b.cols; j++) {
                settings::real s = 0;
                // implement matrix multiplication the naive way here
                for(int k=0; k<a.cols; k++) {
                    s += a.values[i * a.cols + k] * b.values[k * b.cols + j];
                }
                res.values[i * res.cols + j] = s;
            }
        };

        /*auto *new_values = new settings::real[a.rows * b.cols];
        for (int i = 0; i < a.rows * b.cols; i++)
        {
            new_values[i] = 0;
        }

        for (int k = 0; k < b.rows; k++)
        {
            for (int c = 0; c < a.rows * b.cols; c++)
            {
                // make sure this rounds down
                int i = c / b.cols;
                int j = c % b.cols;
                new_values[c] += a.values[i * a.cols + k] *
                                 b.values[b.cols * k + j];
            }
        };

        Matrix res = Matrix(a.rows, b.cols, new_values);*/
        return res;
    }

    Matrix Matrix::operator+(Matrix const &a) const
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            throw std::invalid_argument(GenerateError("Addition", *this, a));
        }

        Matrix res = Matrix(this->rows, this->cols);
        for (int i = 0; i < a.cols * a.rows; i++)
        {
            res.values[i] = this->values[i] + a.values[i];
        }
        return res;
    };

    Matrix Matrix::operator-(Matrix const &a) const
    {

        if (!Matrix::IsSameShape(*this, a))
        {
            a.print_shape();
            this->print_shape();
            throw std::invalid_argument(GenerateError("Subtraction", *this, a));
        }

        // here checkpiont
        Matrix res = Matrix(this->rows, this->cols);
        for (int i = 0; i < a.cols * a.rows; i++)
        {
            res.values[i] = this->values[i] - a.values[i];
        }

        return res;
    }

    Matrix Matrix::operator*(Matrix const &a) const
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            throw std::invalid_argument(
                GenerateError("Elementwise Multiplication", *this, a));
        }

        auto res = Matrix(this->rows, this->cols);
        for (int i = 0; i < a.cols * a.rows; i++)
        {
            res.values[i] = this->values[i] * a.values[i];
        }
        return res;
    }

    Matrix Matrix::operator/(Matrix const &a) const
    {
        if (!Matrix::IsSameShape(*this, a))
        {
           throw std::invalid_argument(GenerateError("Division", *this, a));
        }

        auto res = Matrix(this->rows, this->cols);
        for (int i = 0; i < a.cols * a.rows; i++)
        {
            if(a.values[i] == 0){
                throw std::overflow_error("Division By Zero");
            }

            res.values[i] = this->values[i] / a.values[i];
        }
        return res;
    };

    Matrix &Matrix::operator+=(Matrix const &a)
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            throw std::invalid_argument(GenerateError("Addition", *this, a));
        }

        for (int i = 0; i < a.cols * a.rows; i++)
        {
            this->values[i] += a.values[i];
        }
        return *this;
    }

    Matrix &Matrix::operator-=(Matrix const &a)
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            throw std::invalid_argument(GenerateError("Subtraction", *this, a));
        }

        for (int i = 0; i < a.cols * a.rows; i++)
        {
            this->values[i] -= a.values[i];
        }
        return *this;
    }

    Matrix &Matrix::operator/=(Matrix const &a)
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            throw std::invalid_argument(GenerateError("Division", *this, a));
        }

        for (int i = 0; i < a.cols * a.rows; i++)
        {
            this->values[i] /= a.values[i];
        }
        return *this;
    }

    Matrix &Matrix::operator*=(Matrix const &a)
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            throw std::invalid_argument(
                GenerateError("elementwise-multiplication", *this, a));
        }

        for (int i = 0; i < a.cols * a.rows; i++)
        {
            this->values[i] *= a.values[i];
        }
        return *this;
    }

    bool Matrix::operator==(Matrix const &a) const
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            return false;
        }

        for (int i = 0; i < a.cols * a.rows; i++)
        {
            if (this->values[i] != a.values[i])
            {
                return false;
            }
        }
        return true;
    }

    Matrix Matrix::VectorProduct(Matrix const &a, Matrix const &b)
    {
        if (a.rows != 3 || a.cols != 1)
        {
            throw std::invalid_argument(GenerateError("Vector product", a));
        }

        if (b.rows != 3 || b.cols != 1)
        {
            throw std::invalid_argument(GenerateError("Vector product", b));
        }

        auto c = Matrix(3, 1);
        c.values[0] = a.values[1] * b.values[2] - a.values[2] * b.values[1];
        c.values[1] = a.values[2] * b.values[0] - a.values[0] * b.values[2];
        c.values[2] = a.values[0] * b.values[1] - a.values[1] * b.values[0];
        return c;
    };
    settings::real Matrix::Dot(Matrix const &a, Matrix const &b)
    {
        if (!IsSameShape(a, b))
        {
            throw std::invalid_argument(GenerateError("Dot Product", a, b));
        }

        settings::real c = 0;
        for (int i = 0; i < a.rows * a.cols;  i++)
        {
            c += a.values[i] * b.values[i];
        }
        return c;
    }

    Matrix &Matrix::operator+=(settings::real a)
    {
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            this->values[i] += a;
        }
        return *this;
    };

    Matrix &Matrix::operator-=(settings::real a)
    {
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            this->values[i] -= a;
        }
        return *this;
    };

    Matrix &Matrix::operator*=(settings::real a)
    {
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            this->values[i] *= a;
        }
        return *this;
    };

    Matrix &Matrix::operator/=(settings::real a)
    {
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            this->values[i] /= a;
        }
        return *this;
    };

    Matrix Matrix::operator+(settings::real a) const
    {
        auto c = Matrix(this->rows, this->cols);
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            c.values[i] = values[i] + a;
        }
        return c;
    }

    Matrix Matrix::operator-(settings::real a) const
    {
        auto c = Matrix(this->rows, this->cols);
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            c.values[i] = values[i] - a;
        }
        return c;
    }

    Matrix Matrix::operator*(settings::real a) const
    {
        auto c = Matrix(this->rows, this->cols);
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            c.values[i] = values[i] * a;
        }
        return c;
    }

    Matrix Matrix::operator/(settings::real a) const
    {
        auto c = Matrix(this->rows, this->cols);
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            c.values[i] = values[i] / a;
        }
        return c;
    }

    void Matrix::print() const
    {
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                std::cout << this->values[i * this->cols + j] << " ";
            }
            std::cout << std::endl;
        }
    };

    // This function does not throw an error, when we meet a zero vector. This has been done for performance purposes.
    void Matrix::Normalise() {
        settings::real norm = 0;
        for(int i=0; i<this->rows * this->cols; i++) {
            norm += this->values[i] * this->values[i];
        }
        norm = sqrt(norm);
        for(int i=0; i<this->rows * this->cols; i++) {
            this->values[i] /= norm;
        }
    }

    bool Matrix::IsZero() {
        for(int i=0; i<this->rows * this->cols; i++) {
            if(this->values[i] != 0) {
                return false;
            }
        }
        return true;
    }

    void Matrix::abs() {
        for(int i=0; i<this->rows * this->cols; i++) {
            this->values[i] = std::abs(this->values[i]);
        }
    };

    void Matrix::ApplyMaxVector3(settings::real max_x, settings::real max_y, settings::real max_z) {
        this->values[0] = std::max(this->values[0], max_x);
        this->values[1] = std::max(this->values[1], max_y);
        this->values[2] = std::max(this->values[2], max_z);
    }

    void Matrix::ApplyMinVector3(settings::real min_x, settings::real min_y, settings::real min_z) {
        this->values[0] = std::min(this->values[0], min_x);
        this->values[1] = std::min(this->values[1], min_y);
        this->values[2] = std::min(this->values[2], min_z);
    }

    Matrix Matrix::CreateColumnVec(settings::real val0, settings::real val1, settings::real val2) {
        Matrix a = Matrix(3, 1);
        a.values[0] = val0;
        a.values[1] = val1;
        a.values[2] = val2;
        return a;
    }

    Matrix Matrix::CreateColumnVec(settings::real val0, settings::real val1, settings::real val2, settings::real val3) {
        Matrix a = Matrix(4, 1);
        a.values[0] = val0;
        a.values[1] = val1;
        a.values[2] = val2;
        a.values[3] = val3;
        return a;
    }


    Matrix Matrix::LookAt(const Matrix &eye, const Matrix &center, const Matrix &up) {

        Matrix  f = center - eye;
        f.Normalise();
        Matrix u = up;
        u.Normalise();

        Matrix s = VectorProduct(f, u);
        s.Normalise();

        u = VectorProduct(s, f);

        Matrix Result(4, 4, 1);
        Result(0, 0, s(0, 0));
        Result(1, 0, s(1, 0));
        Result(2,0, s(2, 0));
        Result(0,1, u(0, 0));
        Result(1,1, u(1, 0));
        Result(2,1, u(2, 0));
        Result(0,2,-f(0, 0));
        Result(1,2,-f(1, 0));
        Result(2,2,-f(2, 0));

        Result(3,0,-Matrix::Dot(s, eye));
        Result(3,1,-Matrix::Dot(u, eye));
        Result(3,2, Matrix::Dot(f, eye));
        return Result;
    }

    Matrix Matrix::Perspective(float fovy, float aspect, float znear, float zfar) {

        float const tanHalfFovy = tan(fovy / ( (settings::real) 2.0));

        Matrix Result(4, 4);
        Result(0, 0,1.0/(aspect * tanHalfFovy));
        Result(0, 0, 1.0/(aspect * tanHalfFovy));
        Result(1, 1, 1.0 / (tanHalfFovy));
        Result(2, 2, - (zfar + znear) / (zfar - znear));
        Result(2, 3, -1.0);
        Result(3, 2, - (2.0 * zfar * znear) / (zfar - znear));
        return Result;
    }

    settings::real Matrix::ConvertToRadians(settings::real degrees) {
        return degrees * 0.01745329251994329576923690768489; // PI/180 = 0.017
    }


    void Matrix::Translate4by4Matrix(const Matrix& translation_value) {
        (*this)(3, 0, translation_value(0, 0));
        (*this)(3, 1, translation_value(1, 0));
        (*this)(3, 2, translation_value(2, 0));
    }

    Matrix Matrix::CreateTranslationMatrix(const Matrix &translation_matrix) {
        auto res = Matrix();
        res.cols = 4;
        res.rows = 4;
        auto values = new settings::real[16];
        values[0] = 1;
        values[1] = 0;
        values[2] = 0;
        values[3] = 0;
        values[4] = 0;
        values[5] = 1;
        values[6] = 0;
        values[7] = 0;
        values[8] = 0;
        values[9] = 0;
        values[10] = 1;
        values[11] = 0;
        values[12] = translation_matrix.values[0];
        values[13] = translation_matrix.values[1];
        values[14] = translation_matrix.values[2];
        values[15] = 1;
        res.values = values;
        return res;
    }

} // namespace numerics