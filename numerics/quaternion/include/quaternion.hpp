#ifndef MWORLD_NUMERICS_QUATERNION_QUATERNION_H
#define MWORLD_NUMERICS_QUATERNION_QUATERNION_H

#include <matrix.hpp>
#include <settings.hpp>

namespace numerics
{
   class Quaternion
   {

   public:
      settings::real r;
      settings::real i;
      settings::real j;
      settings::real k;

      /**
     * @brief Construct a new quaternion object
     * 
     * @param r The settings::real part of the quaternion
     * @param i The imaginary part of the quaternion 
     * @param j The j part of quaternion
     * @param k The k part of quaternion
     */
      explicit Quaternion(settings::real r, settings::real i, settings::real j, settings::real k);

      /**
     * @brief Construct a new quaternion object. Uses the copy constructor to achieve this
     * 
     * @param q Quaternion to copy.
     */
      Quaternion(const Quaternion &q);

      void print() const
      {
         std::cout << this->r << "+" << this->i << "i+" << this->j << "j+" << this->k << "k" << std::endl;
      };

      Quaternion &operator=(const Quaternion &q)
      {
         this->r = q.r;
         this->i = q.i;
         this->j = q.j;
         this->k = q.k;
         return *this;
      }

      /**
     * @brief Compares two quaternion. If all values are equal, then returns true
     * 
     * @param a The quaternion to value compare with
     * @return true or false
     */
      bool operator==(const Quaternion &a) const;

      /**
     * @brief Adds two quaternion objects together
     * 
     * @param a A quaternion to add. 
     * @return Quaternion& 
     */
      Quaternion operator+(const Quaternion &a) const;

      /**
     * @brief Subtracts two quaternion objects together
     * 
     * @param a A quaternion to subtract. 
     * @return Quaternion& 
     */
      Quaternion operator-(const Quaternion &a) const;

      /**
     * @brief Multiplies two quaternion objects together
     * 
     * @param a A quaternion to multiply 
     * @return Quaternion& 
     */
      Quaternion operator*(const Quaternion &a) const;

      /**
     * @brief Divides two quaternion objects
     * 
     * @param a A quaternion to divide 
     * @return Quaternion& 
     */
      Quaternion operator/(const Quaternion &a);

      /**
     * @brief Adds a quaternion to the existing one 
     * 
     * @param a A quaternion to add 
     * @return Quaternion& 
     */
      Quaternion operator+=(Quaternion const &a);

      /**
     * @brief Subtracts a quaternion form the existing one 
     * 
     * @param a A quaternion to subtract 
     * @return Quaternion& 
     */
      Quaternion operator-=(Quaternion const &a);

      /**
     * @brief Multiples existing quaternion with given quaternion 
     * 
     * @param a A quaternion to multiply with 
     * @return Quaternion&
     */
      Quaternion operator*=(Quaternion const &a);

      /**
     * @brief Divides existing quaternion by a 
     * 
     * @param a A quaternion to divide with
     * @return Quaternion& 
     */
      Quaternion operator/=(Quaternion const &a);

      /**
     * @brief Add a settings::real to a quaternion
     * 
     * @param a A settings::real number to add to quaternion
     * @return Quaternion& 
     */
      Quaternion operator+(settings::real a) const;

      /**
     * @brief Subtracts quaternion from settings::real
     * 
     * @param a A settings::real number to subtract quaternion by
     * @return Quaternion& 
     */
      Quaternion operator-(settings::real a) const;

      /**
     * @brief Multiply quaternion with settings::real number 
     * 
     * @param a A settings::real number to multiply quaternion by
     * @return Quaternion& 
     */
      Quaternion operator*(settings::real a) const;

      /**
     * @brief Divide quaternion with settings::real number 
     * 
     * @param a A settings::real number to divide quaternion with 
     * @return Quaternion& 
     */
      Quaternion operator/(settings::real a) const;

      /**
     * @brief Add existing quaternion with settings::real number 
     * 
     * @param a A settings::real number to add quaternion with 
     * @return Quaternion& 
     */
      Quaternion operator+=(settings::real a);

      /**
     * @brief Subtract existing quaternion with settings::real number 
     * 
     * @param a A settings::real number to subtract quaternion with 
     * @return Quaternion& 
     */
      Quaternion operator-=(settings::real a);

      /**
     * @brief Multiply existing quaternion with settings::real number 
     * 
     * @param a A settings::real number to add quaternion with 
     * @return Quaternion& 
     */
      Quaternion operator*=(settings::real a);

      /**
     * @brief Divide existing quaternion with settings::real number 
     * 
     * @param a A settings::real number to divide quaternion with 
     * @return Quaternion& 
     */
      Quaternion operator/=(settings::real a);

      /**
     * @brief  Returns the conjugate of a given quaternion
     * 
     * @param a Quaternion to get conjugate of 
     * @return Quaternion& 
     */
      Quaternion static GetConjugate(const Quaternion &a);

      /**
     * @brief  Returns the conjugate of quaternion
     * 
     * @return Quaternion& 
     */
      Quaternion Conjugate();

      /**
     * @brief  Returns the inverse of a given quaternion 
     * 
     * @param a A quaternion to calculate the inverse of
     * @return Quaternion&
     */
      Quaternion static Inverse(const Quaternion &a);

      /**
     * @brief  Returns the inverse of quaternion 
     * 
     * @return Quaternion&
     */
      Quaternion &Inverse();

      /**
     * @brief Calculates the magnitude of given quaternion
     * 
     * @param a A quaternion whos magnitude to calculate
     * @return settings::real 
     */
      settings::real static Magnitude(const Quaternion &a);

      /**
     * @brief Calculates the magnitude of quaternion
     * 
     * @return settings::real
     */
      settings::real Magnitude() const;

      /**
     * @brief Caculates the squared magnitude of a given quaternion. Use this to calculate squared magnitude.
     * 
     * @return settings::real
     */
      settings::real static SquaredMagnitude(const Quaternion &a);

      /**
     * @brief Caculates the squared magnitude quaternion. Use this function to calculate squared magnitude.
     * 
     * @return settings::real
     */
      settings::real SquaredMagnitude() const;

      /**
     * @brief Get the normalised quaternion
     * 
     * @param a Quaternion, whos normal to calculate
     * @return Quaternion& 
     */
      Quaternion static GetNormalised(const Quaternion &a);

      /**
     * @brief Normalises quaternion. Returns a reference to instance
     * 
     * @return Quaternion& 
     */
      Quaternion &Normalise();

      /**
       * @brief Gets the vector representation of a quaternion 
       * 
       * @param a Quaternion to convert to vector representation.
       * @return Matrix& 
       */
      Matrix static ConvertToVector(const Quaternion &a);

      /**
       * @brief Converts a vector to a given Quaternion
       * 
       * @param a Quaternion to convert
       * @return Quaternion& 
       */
      Quaternion static ConvertToQuaternion(const Matrix &a);

      /**
       * @brief Converts a real number to a quaternion
       * 
       * @param real real number to convert
       * @return Quaternion& 
       */
      Quaternion static ConvertToQuaternion(settings::real real);

      /**
       * @brief Get the Matrix Transformation corresponding to given quaternion
       * @see  https://gafferongames.com/post/physics_in_3d/
       * @see https://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToMatrix/index.htm
       * @param a A quaternion
       * @return Matrix& 
       */
      Matrix static GetMatrixTransformation(const Quaternion &a);

      /**
       * @brief Calculates the inverse of the orientation quaternion given.
       * 
       * @param a 
       * @return Matrix& 
       */
      Matrix static GetInverseMatrixTransformation(const Quaternion &a);

      /**
       * @brief Returns the 3by3 inverse Matrix of the Quaternion given. 
       * 
       * @param a 
       * @return Matrix& 
       */
      Matrix static GetInverseOrentationMatrix3(const Quaternion &a);

      /**
       * @brief Returns the 3by3 Matrix of the Quaternion given. 
       * 
       * @param a 
       * @return Matrix& 
       */
      Matrix static GetOrientationMatrix3(const Quaternion &a);
   };

} // namespace numerics
#endif