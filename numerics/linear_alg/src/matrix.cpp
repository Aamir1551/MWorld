#include <matrix.hpp>
#include <math.h>
#include <iostream>
#include <settings.hpp>

//TODO
// 1) pls add this to all generateError throw std::invalid_argument

namespace numerics
{
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

        this->rows = a.rows;
        this->cols = a.cols;
        delete (this->values);
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

    Matrix &Matrix::Reshape(Matrix const a, int new_row, int new_cols)
    {
        if (a.rows * a.cols != new_row * new_cols)
        {
            throw std::invalid_argument("Matrix Shape does not conform for Reshape");
        }
        Matrix *b = new Matrix(new_row, new_cols);
        for (int i = 0; i < a.rows * a.cols; i++)
        {
            b->values[i] = a.values[i];
        }
        return *b;
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

    settings::real Matrix::Norm(Matrix const &a) const
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
        std::string a_shape = "This has shape=(" + std::to_string(a.rows) + "," +
                              std::to_string(a.cols) + ").";

        return msg + " " + a_shape;
    }

    int Matrix::MatMul(Matrix const &a)
    {
        if (this->cols != a.rows)
        {
            throw std::invalid_argument(
                GenerateError("Matrix Multiplication", *this, a));
        }

        settings::real *new_values = new settings::real[this->rows * a.cols];
        for (int i = 0; i < this->rows * a.cols; i++)
        {
            new_values[i] = 0;
        }

        for (int k = 0; k < a.rows; k++)
        {
            for (int c = 0; c < this->rows * a.cols; c++)
            {
                // make sure this rounds down
                int i = c / a.cols;
                int j = c % a.cols;
                new_values[c] += this->values[i * this->cols + k] *
                                 a.values[a.cols * k + j];
            }
        }
        delete[] this->values;
        this->values = new_values;
        this->cols = a.cols;
        return 0;
    };

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
        Matrix *a = new Matrix(n, m);
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
        settings::real *new_values = new settings::real[this->cols * this->rows];
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->cols; j++)
            {
                new_values[j * this->rows + i] = new_values[i * this->cols + j];
            }
        }
        int t = this->cols;

        this->cols = this->rows;
        this->rows = t;
        this->values = new_values;
    }

    Matrix &Matrix::Transpose(Matrix const &a)
    {
        settings::real *new_values = new settings::real[a.cols * a.rows];
        for (int i = 0; i < a.rows; i++)
        {
            for (int j = 0; j < a.cols; j++)
            {
                new_values[j * a.rows + i] = a.values[i * a.cols + j];
            }
        }
        Matrix *res = new Matrix(a.cols, a.rows, new_values);
        return *res;
    }

    Matrix &Matrix::MatMul(Matrix const &a, Matrix const &b)
    {
        if (a.cols != b.rows)
        {
            throw std::invalid_argument(GenerateError("Multiplication", a, b));
        }

        settings::real *new_values = new settings::real[a.rows * b.cols];
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

        Matrix *res = new Matrix(a.rows, b.cols, new_values);
        return *res;
    }

    Matrix &Matrix::operator+(Matrix const &a) const
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            throw std::invalid_argument(GenerateError("Addition", *this, a));
        }

        settings::real *new_values = new settings::real[a.cols * a.rows];
        for (int i = 0; i < a.cols * a.rows; i++)
        {
            new_values[i] = this->values[i] + a.values[i];
        }

        Matrix *res = new Matrix(this->rows, this->cols, new_values);
        return *res;
    };

    Matrix &Matrix::operator-(Matrix const &a) const
    {

        if (!Matrix::IsSameShape(*this, a))
        {
            //pls add this to all generateError
            throw std::invalid_argument(GenerateError("Subtraction", *this, a));
        }

        settings::real *new_values = new settings::real[a.cols * a.rows];
        for (int i = 0; i < a.cols * a.rows; i++)
        {
            new_values[i] = this->values[i] - a.values[i];
        }

        Matrix *res = new Matrix(this->rows, this->cols, new_values);
        return *res;
    }

    Matrix &Matrix::operator*(Matrix const &a) const
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            throw std::invalid_argument(
                GenerateError("Elementwise Multiplication", *this, a));
        }

        settings::real *new_values = new settings::real[a.cols * a.rows];
        for (int i = 0; i < a.cols * a.rows; i++)
        {
            new_values[i] = this->values[i] * a.values[i];
        }
        Matrix *res = new Matrix(this->rows, this->cols, new_values);
        return *res;
    }

    Matrix &Matrix::operator/(Matrix const &a) const
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            std::invalid_argument(GenerateError("Division", *this, a));
        }

        settings::real *new_values = new settings::real[a.cols * a.rows];
        for (int i = 0; i < a.cols * a.rows; i++)
        {
            new_values[i] = this->values[i] / a.values[i];
        }
        Matrix *res = new Matrix(this->rows, this->cols, new_values);
        return *res;
    };

    Matrix &Matrix::operator+=(Matrix const &a)
    {
        if (!Matrix::IsSameShape(*this, a))
        {
            std::invalid_argument(GenerateError("Addition", *this, a));
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
            std::invalid_argument(GenerateError("Subtraction", *this, a));
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
            std::invalid_argument(GenerateError("Division", *this, a));
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

    Matrix &Matrix::VectorProduct(Matrix const &a, Matrix const &b)
    {
        if (a.rows != 3 || a.cols != 1)
        {
            throw std::invalid_argument(GenerateError("vector product", a));
        }

        if (b.rows != 3 || b.cols != 1)
        {
            throw std::invalid_argument(GenerateError("vector product", b));
        }

        Matrix *c = new Matrix(3, 1);
        c->values[0] = a.values[1] * b.values[2] - a.values[2] * b.values[1];
        c->values[1] = a.values[2] * b.values[0] - a.values[0] * b.values[2];
        c->values[2] = a.values[0] * b.values[1] - a.values[1] * b.values[0];
        return *c;
    };
    settings::real Matrix::Dot(Matrix const &a, Matrix const &b)
    {
        if (!IsSameShape(a, b))
        {
            throw std::invalid_argument(GenerateError("Dot Product", a, b));
        }

        settings::real c = 0;
        for (int i = 0; i < a.rows; i++)
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

    Matrix &Matrix::operator+(settings::real a) const
    {
        Matrix *c = new Matrix(this->rows, this->cols);
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            c->values[i] = values[i] + a;
        }
        return *c;
    }

    Matrix &Matrix::operator-(settings::real a) const
    {
        Matrix *c = new Matrix(this->rows, this->cols);
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            c->values[i] = values[i] - a;
        }
        return *c;
    }

    Matrix &Matrix::operator*(settings::real a) const
    {
        Matrix *c = new Matrix(this->rows, this->cols);
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            c->values[i] = values[i] * a;
        }
        return *c;
    }

    Matrix &Matrix::operator/(settings::real a) const
    {
        Matrix *c = new Matrix(this->rows, this->cols);
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            c->values[i] = values[i] / a;
        }
        return *c;
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

    /* Returns 0 if success, otherwise it throws an error or 
if an element is nan, it returns 1 */
    settings::real Matrix::Inv() const
    {
        return 0;
    }

    settings::real Matrix::Determinent() const
    {
        return 0;
    }

    Matrix *Matrix::GetRows() const
    {
        Matrix *rows = (Matrix *)malloc(sizeof(Matrix) * this->rows);
        for (int i = 0; i < this->rows; i++)
        {
            settings::real *values;
            for (int j = 0; j < this->cols; j++)
            {
                values[j] = (*this)(i, j);
            }
            rows[i] = Matrix(1, this->cols, values);
        }
        return rows;
    }

    void Matrix::AddColumn(Matrix const &vec)
    {

        if (vec.cols != 1 || this->rows != vec.rows)
            throw std::invalid_argument("Vector shape does not conform for AddColumn");

        settings::real *new_vals = new settings::real[this->rows * (this->cols + 1)];
        for (int i = 0; i < this->rows * (this->cols + 1); i++)
        {
            if (i > 0 && i % (this->cols + 1) == 0)
            {
                continue;
            }
            new_vals[i] = this->values[i];
        }
        for (int i = 1; i < this->rows + 1; i++)
        {
            new_vals[i * (this->cols + 1)] = vec(i, 0);
        }
        delete this->values;
        this->values = new_vals;
        this->cols += 1;
    }

    void Matrix::AddRow(Matrix const &vec)
    {
        // make sure shape is the same

        if (vec.rows != 1 || this->cols != vec.cols)
            throw std::invalid_argument("Vector shape does not conform for AddColumn");

        settings::real *new_vals = new settings::real[(this->rows + 1) * this->rows];
        for (int i = 0; i < this->cols * this->rows; i++)
        {
            new_vals[i] = this->values[i];
        }
        for (int i = 0; i < this->cols; i++)
        {
            new_vals[this->rows * this->cols + i] = vec(0, i);
        }
        delete this->values;
        this->values = new_vals;
        this->rows += 1;
    }

    Matrix *Matrix::GetColumns() const
    {

        Matrix *cols = (Matrix *)malloc(sizeof(Matrix) * this->cols);
        for (int i = 0; i < this->cols; i++)
        {
            settings::real *values;
            for (int j = 0; j < this->rows; j++)
            {
                values[j] = (*this)(i, j);
            }
            cols[i] = Matrix(1, this->rows, values);
        }
        return cols;
    }

    void Matrix::RemoveRow(int index)
    {

        int size = this->cols * this->rows;
        if (size == 0)
        {
            std::invalid_argument("Matrix is of size zero. Must be more than zero for operation RemoveRow");
        }
        settings::real *new_values = new settings::real[(this->rows - 1) * this->cols];
        for (int i = 0; i < index * this->cols; i++)
        {
            new_values[i] = this->values[i];
        }
        for (int i = (index + 1) * this->cols; i < size; i++)
        {
            new_values[i] = this->values[i];
        }
        delete[] values;
        values = new_values;
        rows--;
    };

    void Matrix::RemoveColumn(int index)
    {
        int size = this->cols * this->rows;
        if (size == 0)
        {
            std::invalid_argument("Matrix is of size zero. Must be more than zero for operation RemoveColumn");
        }
        settings::real *new_values = new settings::real[(this->cols - 1) * this->rows];
        for (int i = 0; i < size; i++)
        {
            if (i % this->cols == index)
            {
                continue;
            }
            new_values[i] = this->values[i];
        }
        delete[] values;
        values = new_values;
        cols--;
    };

} // namespace numerics