#include <math.h>

#include <quaternion.hpp>
#include <settings.hpp>
#include <matrix.hpp>

namespace numerics
{

    Quaternion::Quaternion(settings::real r, settings::real i, settings::real j, settings::real k) : r(r), i(i), j(j), k(k){};

    Quaternion::Quaternion(const Quaternion &q)
    {
        this->r = q.r;
        this->i = q.i;
        this->j = q.j;
        this->k = q.k;
    };

    Quaternion Quaternion::operator+(const Quaternion &a) const
    {
        Quaternion result = Quaternion(this->r + a.r, this->i + a.i, this->j + a.j, this->k + a.k);
        return result;
    };

    Quaternion Quaternion::operator-(const Quaternion &a) const
    {
        Quaternion result = Quaternion(this->r - a.r, this->i - a.i, this->j - a.j, this->k - a.k);
        return result;
    };

    Quaternion Quaternion::operator*(const Quaternion &a) const
    {
        //should this be inline instead???
        settings::real r_component = this->r * a.r - this->i * a.i - this->j * a.j - this->k * a.k;
        settings::real i_component = this->r * a.i + this->i * a.r + this->j * a.k - this->k * a.j;
        settings::real j_component = this->r * a.j - this->i * a.k + this->j * a.r + this->k * a.i;
        settings::real k_component = this->r * a.k + this->i * a.j - this->j * a.i + this->k * a.r;
        Quaternion result = Quaternion(r_component, i_component, j_component, k_component);
        return result;
    }

    Quaternion Quaternion::operator/(const Quaternion &a)
    {
        return (*this) * Quaternion::Inverse(a);
    }

    Quaternion Quaternion::operator*=(Quaternion const &a)
    {
        settings::real r_component = this->r * a.r - this->i * a.i - this->j * a.j - this->k * a.k;
        settings::real i_component = this->r * a.i + this->i * a.r + this->j * a.k - this->k * a.j;
        settings::real j_component = this->r * a.j - this->i * a.k + this->j * a.r + this->k * a.i;
        settings::real k_component = this->r * a.k + this->i * a.j - this->j * a.i + this->k * a.r;
        this->r = r_component;
        this->i = i_component;
        this->j = j_component;
        this->k = k_component;
        return *this;
    }

    Quaternion Quaternion::operator+=(Quaternion const &a)
    {
        this->r += a.r;
        this->i += a.i;
        this->j += a.j;
        this->k += a.k;
        return *this;
    };

    Quaternion Quaternion::operator-=(Quaternion const &a)
    {
        this->r -= a.r;
        this->i -= a.i;
        this->j -= a.j;
        this->k -= a.k;
        return *this;
    };

    Quaternion Quaternion::operator/=(Quaternion const &a)
    {
        *this *= Quaternion::Inverse(a);
        return *this;
    };

    Quaternion Quaternion::GetConjugate(const Quaternion &a)
    {
        Quaternion result = Quaternion(a.r, -a.i, -a.j, -a.k);
        return result;
    };

    Quaternion Quaternion::Conjugate()
    {
        this->i = -this->i;
        this->j = -this->j;
        this->k = -this->k;
        return *this;
    };

    settings::real Quaternion::SquaredMagnitude() const
    {
        return this->r * this->r + this->i * this->i + this->j * this->j + this->k * this->k;
    }

    settings::real Quaternion::SquaredMagnitude(const Quaternion &a)
    {
        return a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k;
    }

    settings::real Quaternion::Magnitude(const Quaternion &a)
    {
        return sqrt(a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k);
    }

    settings::real Quaternion::Magnitude() const
    {
        return sqrt(this->r * this->r + this->i * this->i + this->j * this->j + this->k * this->k);
    }

    Quaternion Quaternion::Inverse(const Quaternion &a)
    {
        settings::real mag = a.SquaredMagnitude();
        Quaternion result = Quaternion(a.r / mag, -a.i / mag, -a.j / mag, -a.k / mag);
        return result;
    };

    Quaternion &Quaternion::Inverse() {
        settings::real mag = this->SquaredMagnitude();
        this->r = this->r / mag;
        this->i = -this->i / mag;
        this->j = -this->j / mag;
        this->k = -this->k / mag;
        return *this;
    };

    bool Quaternion::operator==(const Quaternion &a) const
    {
        return this->r == a.r && a.i == this->i && a.j == this->j && this->k == a.k;
    };

    Quaternion Quaternion::operator+=(settings::real a)
    {
        this->r += a;
        return *this;
    };

    Quaternion Quaternion::operator-=(settings::real a)
    {
        this->r -= a;
        return *this;
    };

    Quaternion Quaternion::operator*=(settings::real a)
    {
        this->r *= a;
        this->i *= a;
        this->j *= a;
        this->k *= a;
        return *this;
    };

    Quaternion Quaternion::operator/=(settings::real a)
    {
        this->r /= a;
        this->i /= a;
        this->j /= a;
        this->k /= a;
        return *this;
    };

    Quaternion Quaternion::operator*(settings::real a) const
    {
        Quaternion result = Quaternion(this->r * a, this->i * a, this->j * a, this->k * a);
        return result;
    }

    Quaternion Quaternion::operator/(settings::real a) const
    {
        Quaternion result = Quaternion(this->r / a, this->i / a, this->j / a, this->k / a);
        return result;
    }

    Quaternion Quaternion::operator+(settings::real a) const
    {
        Quaternion result = Quaternion(this->r + a, this->i, this->j, this->k);
        return result;
    }

    Quaternion Quaternion::operator-(settings::real a) const
    {
        Quaternion result = Quaternion(this->r - a, this->i, this->j, this->k);
        return result;
    }

    Quaternion Quaternion::GetNormalised(const Quaternion &a)
    {
        settings::real mag = a.Magnitude();
        Quaternion result = Quaternion(a.r / mag, a.i / mag, a.j / mag, a.k / mag);
        return result;
    };

    Quaternion &Quaternion::Normalise()
    {
        settings::real mag = this->Magnitude();
        this->r /= mag;
        this->i /= mag;
        this->j /= mag;
        this->k /= mag;
        return *this;
    };

    Matrix Quaternion::ConvertToVector(const Quaternion &a)
    {
        Matrix result = Matrix(4, 1);
        auto v = result.values;
        v[0] = a.r;
        v[1] = a.i;
        v[2] = a.j;
        v[3] = a.k;
        return result;
    }

    Quaternion Quaternion::ConvertToQuaternion(const Matrix &a)
    {
        settings::real const *new_values = a.GetValues();
        Quaternion result = Quaternion(new_values[0], new_values[1], new_values[2], new_values[3]);
        return result;
    }

    Matrix Quaternion::GetMatrixTransformation(const Quaternion &a)
    {
        settings::real qw = a.r;
        settings::real qx = a.i;
        settings::real qy = a.j;
        settings::real qz = a.k;

        Matrix result = Matrix(4, 4);
        auto values = result.values;

        values[0] = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
        values[1] = 2.0f * qx * qy - 2.0f * qz * qw;
        values[2] = 2.0f * qx * qz + 2.0f * qy * qw;
        values[3] = 0.0f;

        values[4] = 2.0f * qx * qy + 2.0f * qz * qw;
        values[5] = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
        values[6] = 2.0f * qy * qz - 2.0f * qx * qw;
        values[7] = 0.0f;

        values[8] = 2.0f * qx * qz - 2.0f * qy * qw;
        values[9] = 2.0f * qy * qz + 2.0f * qx * qw;
        values[10] = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;
        values[11] = 0.0f;

        values[12] = 0.0f;
        values[13] = 0.0f;
        values[14] = 0.0f;
        values[15] = 1.0f;

        return result;
    };

    Matrix Quaternion::GetInverseMatrixTransformation(const Quaternion &a)
    {

        settings::real qw = a.r;
        settings::real qx = a.i;
        settings::real qy = a.j;
        settings::real qz = a.k;

        Matrix result = Matrix(4, 4);
        auto values = result.values;

        values[0] = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
        values[4] = 2.0f * qx * qy - 2.0f * qz * qw;
        values[8] = 2.0f * qx * qz + 2.0f * qy * qw;
        values[3] = 0.0f;

        values[1] = 2.0f * qx * qy + 2.0f * qz * qw;
        values[5] = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
        values[9] = 2.0f * qy * qz - 2.0f * qx * qw;
        values[7] = 0.0f;

        values[2] = 2.0f * qx * qz - 2.0f * qy * qw;
        values[6] = 2.0f * qy * qz + 2.0f * qx * qw;
        values[10] = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;
        values[11] = 0.0f;

        values[12] = 0.0f;
        values[13] = 0.0f;
        values[14] = 0.0f;
        values[15] = 1.0f;

        return result;
    };

    Quaternion Quaternion::ConvertToQuaternion(settings::real real)
    {
        Quaternion result = Quaternion(real, 0, 0, 0);
        return result;
    };

    Matrix Quaternion::GetInverseOrentationMatrix3(const Quaternion &a)
    {
        settings::real qw = a.r;
        settings::real qx = a.i;
        settings::real qy = a.j;
        settings::real qz = a.k;

        Matrix result = Matrix(3, 3);
        auto values = result.values;

        values[0] = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
        values[3] = 2.0f * qx * qy - 2.0f * qz * qw;
        values[6] = 2.0f * qx * qz + 2.0f * qy * qw;

        values[1] = 2.0f * qx * qy + 2.0f * qz * qw;
        values[4] = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
        values[7] = 2.0f * qy * qz - 2.0f * qx * qw;

        values[2] = 2.0f * qx * qz - 2.0f * qy * qw;
        values[5] = 2.0f * qy * qz + 2.0f * qx * qw;
        values[8] = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;

        return result;
    };

    Matrix Quaternion::GetOrientationMatrix3(const Quaternion &a)
    {

        settings::real qw = a.r;
        settings::real qx = a.i;
        settings::real qy = a.j;
        settings::real qz = a.k;

        Matrix result = Matrix(3, 3);
        auto values = result.values;

        values[0] = 1.0f - 2.0f * qy * qy - 2.0f * qz * qz;
        values[1] = 2.0f * qx * qy - 2.0f * qz * qw;
        values[2] = 2.0f * qx * qz + 2.0f * qy * qw;

        values[3] = 2.0f * qx * qy + 2.0f * qz * qw;
        values[4] = 1.0f - 2.0f * qx * qx - 2.0f * qz * qz;
        values[5] = 2.0f * qy * qz - 2.0f * qx * qw;

        values[6] = 2.0f * qx * qz - 2.0f * qy * qw;
        values[7] = 2.0f * qy * qz + 2.0f * qx * qw;
        values[8] = 1.0f - 2.0f * qx * qx - 2.0f * qy * qy;

        return result;
    };

} // namespace numerics