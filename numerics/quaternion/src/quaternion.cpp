#include <quaternion.hpp>
#include <math.h>

Quaternion::Quaternion(int r, int i, int j, int k) : r(r), i(i), j(j), k(k){};

Quaternion &Quaternion::operator+(Quaternion &a)
{
    Quaternion *result = new Quaternion(this->r + a.r, this->i + a.i, this->j + a.j, this->k + a.k);
    return *result;
};

Quaternion &Quaternion::operator-(Quaternion &a)
{
    Quaternion *result = new Quaternion(this->r - a.r, this->i - a.i, this->j - a.j, this->k - a.k);
    return *result;
};

Quaternion &Quaternion::operator*(Quaternion &a)
{
    //should this be inline instead???
    real r_component = this->r * a.r - this->i * a.i - this->j * a.j - this->k * a.k;
    real i_component = this->r * a.i + this->i * a.r + this->j * a.k - this->k * a.j;
    real j_component = this->r * a.j - this->i * a.k + this->j * a.r + this->k * a.i;
    real k_component = this->r * a.k + this->i * a.j - this->j * a.i + this->k * a.r;
    Quaternion *result = new Quaternion(r_component, i_component, j_component, k_component);
    return *result;
}

Quaternion &Quaternion::operator/(Quaternion &a)
{
    return (*this) * Quaternion::Inverse(a);
}

Quaternion &Quaternion::operator*=(Quaternion &a)
{
    real r_component = this->r * a.r - this->i * a.i - this->j * a.j - this->k * a.k;
    real i_component = this->r * a.i + this->i * a.r + this->j * a.k - this->k * a.j;
    real j_component = this->r * a.j - this->i * a.k + this->j * a.r + this->k * a.i;
    real k_component = this->r * a.k + this->i * a.j - this->j * a.i + this->k * a.r;
    this->r = r_component;
    this->i = i_component;
    this->j = j_component;
    this->k = k_component;
    return *this;
}

Quaternion &Quaternion::operator+=(Quaternion &a)
{
    this->r += a.r;
    this->i += a.i;
    this->j += a.j;
    this->k += a.k;
    return *this;
};

Quaternion &Quaternion::operator-=(Quaternion &a)
{
    this->r -= a.r;
    this->i -= a.i;
    this->j -= a.j;
    this->k -= a.k;
    return *this;
};

Quaternion &Quaternion::operator/=(Quaternion &a)
{
    *this *= Quaternion::Inverse(a);
};

Quaternion inline &Quaternion::GetConjugate(Quaternion &a)
{
    Quaternion *result = new Quaternion(a.r, -a.i, -a.j, -a.k);
    return *result;
};

Quaternion inline &Quaternion::Conjugate()
{
    this->r = this->r;
    this->i = -this->i;
    this->j = -this->j;
    this->k = -this->k;
    return *this;
};

real inline Quaternion::SquaredMagnitude() const
{
    return this->r * this->r + this->i * this->i + this->j * this->j + this->k * this->k;
}

real inline Quaternion::SquaredMagnitude(Quaternion &a)
{
    return a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k;
}

real inline Quaternion::Magnitude(Quaternion &a)
{
    return sqrt(a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k);
}

real Quaternion::Magnitude() const
{
    return sqrt(this->r * this->r + this->i * this->i + this->j * this->j + this->k * this->k);
}

Quaternion inline &Quaternion::Inverse(Quaternion &a)
{
    real mag = a.SquaredMagnitude();
    Quaternion *result = new Quaternion(a.r / mag, -a.i / mag, -a.j / mag, -a.k / mag);
    return *result;
};

Quaternion inline &Quaternion::Inverse()
{
    real mag = this->SquaredMagnitude();
    this->r = this->r / mag;
    this->i = -this->i / mag;
    this->j = -this->j / mag;
    this->k = -this->k / mag;
    return *this;
};

bool Quaternion::operator==(Quaternion &a)
{
    return this->r == a.r && a.i == this->i && a.j == this->j && this->k == a.k;
};

Quaternion inline &Quaternion::operator+=(real a)
{
    this->r += a;
    return *this;
};

Quaternion inline &Quaternion::operator-=(real a)
{
    this->r -= a;
    return *this;
};

Quaternion inline &Quaternion::operator*=(real a)
{
    this->r *= a;
    this->i *= a;
    this->j *= a;
    this->k *= a;
    return *this;
};

Quaternion inline &Quaternion::operator/=(real a)
{
    this->r /= a;
    this->i /= a;
    this->j /= a;
    this->k /= a;
    return *this;
};

Quaternion inline &Quaternion::GetNormalised(Quaternion &a)
{
    real mag = a.Magnitude();
    Quaternion *result = new Quaternion(a.r / mag, a.i / mag, a.j / mag, a.k / mag);
    return *result;
};

Quaternion inline &Quaternion::Normalise()
{
    real mag = this->Magnitude();
    this->r /= mag;
    this->i /= mag;
    this->j /= mag;
    this->k /= mag;
    return *this;
};

Matrix &Quaternion::ConvertToMatrix(Quaternion &a)
{
    real *new_values = new real[4];
    new_values[0] = a.r;
    new_values[1] = a.i;
    new_values[2] = a.j;
    new_values[3] = a.k;
    Matrix *result = new Matrix(4, 1, new_values);
    return *result;
}

Quaternion &Quaternion::ConvertToQuaternion(Matrix &a)
{
    real const *new_values = a.getValues();
    Quaternion *result = new Quaternion(new_values[0], new_values[1], new_values[2], new_values[3]);
    return *result;
}

//add a move operator
