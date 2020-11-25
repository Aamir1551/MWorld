#include <matrix.h>
#include <math.h>
#include <iostream>

#if defined(__GNUC__) || defined(__ICL) || defined(__clang__)
#define EXPECT(x, y) (__builtin_expect((x), (y)))
#else
#define EXPECT(x, y) (x)
#endif

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols)
{
    this->values = new real[rows * cols];
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
    this->values = new real[rows * cols];

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
    this->values = new real[rows * cols];
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

Matrix::Matrix(const Matrix &a) //copy constructor
{
    this->rows = a.rows;
    this->cols = a.cols;
    delete (this->values);
    this->values = new real[rows * cols];

    for (int i = 0; i < a.rows * a.cols; i++)
    {
        this->values[i] = a.values[i];
    }
}

// Returns the squared euclidean norm of a matrix
real Matrix::squaredNorm(Matrix const &a)
{
    real sum = 0;
    for (int i = 0; i < a.rows * a.cols; i++)
    {
        sum += a.values[i] * a.values[i];
    }
    return (sum);
};

/* returns the euclidean norm of a matrix. Function havn't used a function call 
to norm since that would add too many function calls, and norm is used allot, 
hence would become a bottleneck in calculations and slow down simulation. */
real Matrix::norm(Matrix const &a)
{
    real sum = 0;
    for (int i = 0; i < a.rows * a.cols; i++)
    {
        sum += a.values[i] * a.values[i];
    }
    return sqrt(sum);
};

std::string Matrix::generateError(std::string operation,
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

std::string Matrix::generateError(std::string operation, Matrix const &a)
{
    std::string msg = "Matrix shapes do not conform for " + operation + ".";
    std::string a_shape = "This has shape=(" + std::to_string(a.rows) + "," +
                          std::to_string(a.cols) + ").";

    return msg + " " + a_shape;
}

real Matrix::operator()(int row, int col)
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

int Matrix::MatMul(Matrix const &a)
{
    if (this->cols != a.rows)
    {
        throw std::invalid_argument(
            generateError("Matrix Multiplication", *this, a));
    }

    real *new_values = new real[this->rows * a.cols];
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

Matrix &Matrix::createMatrixArange(int n, int m)
{
    Matrix *a = new Matrix(n, m);
    for (int i = 0; i < m * n; i++)
    {
        a->values[i] = i;
    }
    return *a;
};

Matrix::Matrix(int rows, int cols, real *values) : rows(rows),
                                                   cols(cols), values(values)
{
    this->rows = rows;
    this->cols = cols;
    this->values = new real[rows * cols];
    for (int i = 0; i < rows * cols; i++)
    {
        this->values[i] = values[i];
    }
};

int Matrix::transpose()
{
    real *new_values = new real[this->cols * this->rows];
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
    return 0;
}

Matrix &Matrix::Transpose(Matrix const &a)
{
    real *new_values = new real[a.cols * a.rows];
    for (int i = 0; i < a.rows; i++)
    {
        for (int j = 0; j < a.cols; j++)
        {
            new_values[j * a.rows + i] = a.values[i * a.cols + j];
        }
    }
    int t = a.cols;

    Matrix *res = new Matrix(a.rows, a.cols, new_values);
    return *res;
}

Matrix &Matrix::MatMul(Matrix const &a, Matrix const &b)
{
    if (a.cols != b.rows)
    {
        throw std::invalid_argument(generateError("Multiplication", a, b));
    }

    real *new_values = new real[a.rows * b.cols];
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
    if (!Matrix::same_shape(*this, a))
    {
        throw std::invalid_argument(generateError("Addition", *this, a));
    }

    real *new_values = new real[a.cols * a.rows];
    for (int i = 0; i < a.cols * a.rows; i++)
    {
        new_values[i] = this->values[i] + a.values[i];
    }

    Matrix *res = new Matrix(this->rows, this->cols, new_values);
    return *res;
};

Matrix &Matrix::operator-(Matrix const &a) const
{
    if (!Matrix::same_shape(*this, a))
    {
        generateError("Subtraction", *this, a);
    }

    real *new_values = new real[a.cols * a.rows];
    for (int i = 0; i < a.cols * a.rows; i++)
    {
        new_values[i] = this->values[i] - a.values[i];
    }

    Matrix *res = new Matrix(this->rows, this->cols, new_values);
    return *res;
}

Matrix &Matrix::operator*(Matrix const &a) const
{
    if (!Matrix::same_shape(*this, a))
    {
        throw std::invalid_argument(
            generateError("Elementwise Multiplication", *this, a));
    }

    real *new_values = new real[a.cols * a.rows];
    for (int i = 0; i < a.cols * a.rows; i++)
    {
        new_values[i] = this->values[i] * a.values[i];
    }
    Matrix *res = new Matrix(this->rows, this->cols, new_values);
    return *res;
}

Matrix &Matrix::operator/(Matrix const &a) const
{
    if (!Matrix::same_shape(*this, a))
    {
        std::invalid_argument(generateError("Division", *this, a));
    }

    real *new_values = new real[a.cols * a.rows];
    for (int i = 0; i < a.cols * a.rows; i++)
    {
        new_values[i] = this->values[i] / a.values[i];
    }
    Matrix *res = new Matrix(this->rows, this->cols, new_values);
    return *res;
};

Matrix &Matrix::operator+=(Matrix const &a)
{
    if (!Matrix::same_shape(*this, a))
    {
        std::invalid_argument(generateError("Addition", *this, a));
    }

    for (int i = 0; i < a.cols * a.rows; i++)
    {
        this->values[i] += a.values[i];
    }
    return *this;
}

Matrix &Matrix::operator-=(Matrix const &a)
{
    if (!Matrix::same_shape(*this, a))
    {
        std::invalid_argument(generateError("Subtraction", *this, a));
    }

    for (int i = 0; i < a.cols * a.rows; i++)
    {
        this->values[i] -= a.values[i];
    }
    return *this;
}

Matrix &Matrix::operator/=(Matrix const &a)
{
    if (!Matrix::same_shape(*this, a))
    {
        std::invalid_argument(generateError("Division", *this, a));
    }

    for (int i = 0; i < a.cols * a.rows; i++)
    {
        this->values[i] /= a.values[i];
    }
    return *this;
}

Matrix &Matrix::operator*=(Matrix const &a)
{
    if (!Matrix::same_shape(*this, a))
    {
        throw std::invalid_argument(
            generateError("elementwise-multiplication", *this, a));
    }

    for (int i = 0; i < a.cols * a.rows; i++)
    {
        this->values[i] *= a.values[i];
    }
    return *this;
}

bool Matrix::operator==(Matrix const &a) const
{
    if (!Matrix::same_shape(*this, a))
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

Matrix &Matrix::vectorProduct(Matrix const &a, Matrix const &b)
{
    if (a.rows != 3 || a.cols != 1)
    {
        throw std::invalid_argument(generateError("vector product", a));
    }

    if (b.rows != 3 || b.cols != 1)
    {
        generateError("vector product", b);
    }

    Matrix *c = new Matrix(1, 3);
    c->values[0] = a.values[1] * b.values[2] - a.values[2] * b.values[1];
    c->values[1] = a.values[2] * b.values[0] - a.values[0] * b.values[2];
    c->values[2] = a.values[0] * b.values[1] - a.values[1] * b.values[0];
    return *c;
};
real Matrix::dotProduct(Matrix const &a, Matrix const &b)
{
    if (!same_shape(a, b))
    {
        throw std::invalid_argument(generateError("Dot Product", a, b));
    }

    real c = 0;
    for (int i = 0; i < a.rows; i++)
    {
        c += a.values[i] + b.values[i];
    }
    return c;
}

Matrix &Matrix::operator+=(real a)
{
    for (int i = 0; i < this->cols * this->rows; i++)
    {
        this->values[i] += a;
    }
    return *this;
};

Matrix &Matrix::operator-=(real a)
{
    for (int i = 0; i < this->cols * this->rows; i++)
    {
        this->values[i] -= a;
    }
    return *this;
};

Matrix &Matrix::operator*=(real a)
{
    for (int i = 0; i < this->cols * this->rows; i++)
    {
        this->values[i] *= a;
    }
    return *this;
};

Matrix &Matrix::operator/=(real a)
{
    for (int i = 0; i < this->cols * this->rows; i++)
    {
        this->values[i] /= a;
    }
    return *this;
};

Matrix &Matrix::operator+(real a) const
{
    Matrix *c = new Matrix(this->rows, this->cols);
    for (int i = 0; i < this->cols * this->rows; i++)
    {
        c->values[i] = values[i] + a;
    }
    return *c;
}

Matrix &Matrix::operator-(real a) const
{
    Matrix *c = new Matrix(this->rows, this->cols);
    for (int i = 0; i < this->cols * this->rows; i++)
    {
        c->values[i] = values[i] - a;
    }
    return *c;
}

Matrix &Matrix::operator*(real a) const
{
    Matrix *c = new Matrix(this->rows, this->cols);
    for (int i = 0; i < this->cols * this->rows; i++)
    {
        c->values[i] = values[i] * a;
    }
    return *c;
}

Matrix &Matrix::operator/(real a) const
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
int Matrix::inv()
{
    return 0;
}

int Matrix::determinant() const
{
    return 0;
}
