#ifndef QUATERNION_H
#define QUATERNION_H
#include "../linearAlg/src/matrix.h"
#include "../utils/settings.h"
class Quaternion
{

public:
    real r; //no need to make any of these values private
    real i;
    real j;
    real k;

    Quaternion(int r, int i, int j, int k);

    Quaternion inline &operator+(Quaternion &a);
    Quaternion inline &operator-(Quaternion &a);
    Quaternion inline &operator*(Quaternion &a);
    Quaternion inline &operator/(Quaternion &a);

    Quaternion inline &operator+=(Quaternion &a);
    Quaternion inline &operator-=(Quaternion &a);
    Quaternion inline &operator*=(Quaternion &a);
    Quaternion inline &operator/=(Quaternion &a);

    Quaternion inline &operator+(real a);
    Quaternion inline &operator-(real a);
    Quaternion inline &operator*(real a);
    Quaternion inline &operator/(real a);

    Quaternion inline &operator+=(real a);
    Quaternion inline &operator-=(real a);
    Quaternion inline &operator*=(real a);
    Quaternion inline &operator/=(real a);

    Quaternion inline static &getConjugate(Quaternion &a);
    Quaternion inline &conjugate();

    Quaternion inline static &inverse(Quaternion &a);
    Quaternion inline &inverse();

    real inline static magnitude(Quaternion &a);
    real inline magnitude() const;

    real inline static squareMagnitude(Quaternion &a);
    real inline squareMagnitude() const;

    Quaternion inline static &get_normalised(Quaternion &a);
    Quaternion inline &normalise();

    bool operator==(Quaternion &a);
    Matrix static &convertToMatrix(Quaternion &a);

    Quaternion &convertToQuaternion(Matrix &a);

    //add feature to convert a real to a quaternion
};

#endif