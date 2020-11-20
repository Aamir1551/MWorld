#include "quaternion.h"
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
    return (*this) * Quaternion::inverse(a);
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
    *this *= Quaternion::inverse(a);
};

Quaternion inline &Quaternion::getConjugate(Quaternion &a)
{
    Quaternion *result = new Quaternion(a.r, -a.i, -a.j, -a.k);
    return *result;
};

Quaternion inline &Quaternion::conjugate()
{
    this->r = this->r;
    this->i = -this->i;
    this->j = -this->j;
    this->k = -this->k;
    return *this;
};

real inline Quaternion::squareMagnitude() const
{
    return this->r * this->r + this->i * this->i + this->j * this->j + this->k * this->k;
}

real inline Quaternion::squareMagnitude(Quaternion &a)
{
    return a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k;
}

real inline Quaternion::magnitude(Quaternion &a)
{
    return sqrt(a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k);
}

real inline Quaternion::magnitude() const
{
    return sqrt(this->r * this->r + this->i * this->i + this->j * this->j + this->k * this->k);
}

Quaternion inline &Quaternion::inverse(Quaternion &a)
{
    real mag = a.squareMagnitude();
    Quaternion *result = new Quaternion(a.r / mag, -a.i / mag, -a.j / mag, -a.k / mag);
    return *result;
};

Quaternion inline &Quaternion::inverse()
{
    real mag = this->squareMagnitude();
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
    return *this;
};

Quaternion inline &Quaternion::operator/=(real a)
{
    this->r /= a;
    return *this;
};

Quaternion inline &Quaternion::get_normalised(Quaternion &a)
{
    real mag = a.magnitude();
    Quaternion *result = new Quaternion(a.r / mag, a.i / mag, a.j / mag, a.k / mag);
    return *result;
};

Quaternion inline &Quaternion::normalise()
{
    real mag = this->magnitude();
    this->r /= mag;
    this->i /= mag;
    this->j /= mag;
    this->k /= mag;
    return *this;
};

Matrix &Quaternion::convertToMatrix(Quaternion &a)
{
    real *new_values = new real[4];
    new_values[0] = a.r;
    new_values[1] = a.i;
    new_values[2] = a.j;
    new_values[3] = a.k;
    Matrix *result = new Matrix(4, 1, new_values);
    return *result;
}

Quaternion &Quaternion::convertToQuaternion(Matrix &a)
{
    real const *new_values = a.getValues();
    Quaternion *result = new Quaternion(new_values[0], new_values[1], new_values[2], new_values[3]);
    return *result;
}

//add a move operator
