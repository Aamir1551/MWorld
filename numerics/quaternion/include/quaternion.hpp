#ifndef QUATERNION_H
#define QUATERNION_H

#include <matrix.h>
#include <settings.h>
class Quaternion
{

public:
    real r;
    real i;
    real j;
    real k;

    /**
     * @brief Construct a new quaternion object
     * 
     * @param r The real part of the quaternion
     * @param i The imaginary part of the quaternion 
     * @param j The j part of quaternion
     * @param k The k part of quaternion
     */
    Quaternion(int r, int i, int j, int k);

    /**
     * @brief Compares two quaternion. If all values are equal, then returns true
     * 
     * @param a The quaternion to value compare with
     * @return true or false
     */
    bool operator==(Quaternion &a);

    /**
     * @brief Adds two quaternion objects together
     * 
     * @param a A quaternion to add. 
     * @return Quaternion& 
     */
    Quaternion inline &operator+(Quaternion &a);

    /**
     * @brief Subtracts two quaternion objects together
     * 
     * @param a A quaternion to subtract. 
     * @return Quaternion& 
     */
    Quaternion inline &operator-(Quaternion &a);

    /**
     * @brief Multiplies two quaternion objects together
     * 
     * @param a A quaternion to multiply 
     * @return Quaternion& 
     */
    Quaternion inline &operator*(Quaternion &a);

    /**
     * @brief Divides two quaternion objects
     * 
     * @param a A quaternion to divide 
     * @return Quaternion& 
     */
    Quaternion inline &operator/(Quaternion &a);

    /**
     * @brief Adds a quaternion to the existing one 
     * 
     * @param a A quaternion to add 
     * @return Quaternion& 
     */
    Quaternion inline &operator+=(Quaternion &a);

    /**
     * @brief Subtracts a quaternion form the existing one 
     * 
     * @param a A quaternion to subtract 
     * @return Quaternion& 
     */
    Quaternion inline &operator-=(Quaternion &a);

    /**
     * @brief Multiples existing quaternion with given quaternion 
     * 
     * @param a A quaternion to multiply with 
     * @return Quaternion&
     */
    Quaternion inline &operator*=(Quaternion &a);

    /**
     * @brief Divides existing quaternion by a 
     * 
     * @param a A quaternion to divide with
     * @return Quaternion& 
     */
    Quaternion inline &operator/=(Quaternion &a);

    /**
     * @brief Add a real to a quaternion
     * 
     * @param a A real number to add to quaternion
     * @return Quaternion& 
     */
    Quaternion inline &operator+(settings::real a);

    /**
     * @brief Subtracts quaternion from real
     * 
     * @param a A real number to subtract quaternion by
     * @return Quaternion& 
     */
    Quaternion inline &operator-(settings::real a);

    /**
     * @brief Multiply quaternion with real number 
     * 
     * @param a A real number to multiply quaternion by
     * @return Quaternion& 
     */
    Quaternion inline &operator*(real a);

    /**
     * @brief Divide quaternion with real number 
     * 
     * @param a A real number to divide quaternion with 
     * @return Quaternion& 
     */
    Quaternion inline &operator/(real a);

    /**
     * @brief Add existing quaternion with real number 
     * 
     * @param a A real number to add quaternion with 
     * @return Quaternion& 
     */
    Quaternion inline &operator+=(real a);

    /**
     * @brief Subtract existing quaternion with real number 
     * 
     * @param a A real number to subtract quaternion with 
     * @return Quaternion& 
     */
    Quaternion inline &operator-=(real a);

    /**
     * @brief Multiply existing quaternion with real number 
     * 
     * @param a A real number to add quaternion with 
     * @return Quaternion& 
     */
    Quaternion inline &operator*=(real a);

    /**
     * @brief Divide existing quaternion with real number 
     * 
     * @param a A real number to divide quaternion with 
     * @return Quaternion& 
     */
    Quaternion inline &operator/=(real a);

    /**
     * @brief  Returns the conjugate of a given quaternion
     * 
     * @param a Quaternion to get conjugate of 
     * @return Quaternion& 
     */
    Quaternion inline static &GetConjugate(Quaternion &a);

    /**
     * @brief  Returns the conjugate of quaternion
     * 
     * @return Quaternion& 
     */
    Quaternion inline &Conjugate();

    /**
     * @brief  Returns the inverse of a given quaternion 
     * 
     * @param a A quaternion to calculate the inverse of
     * @return Quaternion&
     */
    Quaternion inline static &Inverse(Quaternion &a);

    /**
     * @brief  Returns the inverse of quaternion 
     * 
     * @return Quaternion&
     */
    Quaternion inline &Inverse();

    /**
     * @brief Calculates the magnitude of given quaternion
     * 
     * @param a A quaternion whos magnitude to calculate
     * @return real 
     */
    real inline static Magnitude(Quaternion &a);

    /**
     * @brief Calculates the magnitude of quaternion
     * 
     * @return real
     */
    real Magnitude() const;

    /**
     * @brief Caculates the squared magnitude of a given quaternion. Use this to calculate squared magnitude.
     * 
     * @return real
     */
    real inline static SquaredMagnitude(Quaternion &a);

    /**
     * @brief Caculates the squared magnitude quaternion. Use this function to calculate squared magnitude.
     * 
     * @return real
     */
    real inline SquaredMagnitude() const;

    /**
     * @brief Get the normalised quaternion
     * 
     * @param a Quaternion, whos normal to calculate
     * @return Quaternion& 
     */
    Quaternion inline static &GetNormalised(Quaternion &a);

    /**
     * @brief Normalises quaternion. Returns a reference to instance
     * 
     * @return Quaternion& 
     */
    Quaternion inline &Normalise();

    Matrix static &ConvertToMatrix(Quaternion &a);

    Quaternion &ConvertToQuaternion(Matrix &a);

    //add feature to convert a real to a quaternion
    Quaternion &ConvertToQuaternion(real real);
};

#endif