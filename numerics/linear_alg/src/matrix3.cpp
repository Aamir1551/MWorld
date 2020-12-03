#include <matrix.hpp>
#include <matrix3.hpp>
#include <settings.hpp>

namespace numerics
{
    Matrix3::Matrix3() : Matrix(3, 3){};
    int Matrix3::inv()
    {
        int a = this->values[0];
        int b = this->values[1];
        int c = this->values[2];
        int d = this->values[3];
        int e = this->values[4];
        int f = this->values[5];
        int g = this->values[6];
        int h = this->values[7];
        int i = this->values[8];

        /*
    (inv M) * det(M) = 
    ei − fh ch − bi bf − ce
    fg − di ai − cg cd − af
    dh − eg bg − ah ae − bd 
    */

        this->values[0] = e * i - f * h;
        this->values[1] = c * h - b * i;
        this->values[2] = b * f - c * e;
        this->values[3] = f * g - d * i;
        this->values[4] = a * i - c * g;
        this->values[5] = c * d - a * f;
        this->values[6] = d * h - e * g;
        this->values[7] = b * g - a * h;
        this->values[8] = a * e - b * d;

        settings::real det = a * e * i + d * h * c + g * b * f - a * h * f - g * e * c - d * b * i;
        for (int i = 0; i < 9; i++)
        {
            this->values[i] /= det;
        }

        return 0;
    }

    int Matrix3::determinant() const
    {
        //detM = aei + dhc + gbf − ahf − gec − dbi
        int a = this->values[0];
        int b = this->values[1];
        int c = this->values[2];
        int d = this->values[3];
        int e = this->values[4];
        int f = this->values[5];
        int g = this->values[6];
        int h = this->values[7];
        int i = this->values[8];

        settings::real det = a * e * i + d * h * c + g * b * f - a * h * f - g * e * c - d * b * i;
    }
} // namespace numerics