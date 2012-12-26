/*
 * Copyright (C) 2012 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
 *
 * This file is part of Pexeso3D.
 *
 * Pexeso3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Pexeso3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Pexeso3D.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * Definition of a 3D vector
 */

#ifndef VECTOR3D_H
#define VECTOR3D_H


class QDebug;

namespace Maths {

/**
 * Three dimensional vector
 */
struct SVector3D
{
  float x;  /// x coordinate
  float y;  /// y coordinate
  float z;  /// z coordinate

  /**
   * Constructor
   */
  SVector3D(void)
  {
  }

  /**
   * Constructor
   *
   * @param x the x coordinate position
   * @param y the y coordinate position
   * @param z the z coordinate position
   */
  SVector3D(float x, float y, float z)
    : x(x), y(y), z(z)
  {
  }

  /**
   * This function will get the vector size
   */
  float getLength(void) const
  {
    return sqrt(x * x + y * y + z * z);
  }

  /**
   * This function will return the squared vector length
   */
  float getLengthSquared(void) const
  {
    return (x * x + y * y + z * z);
  }

  /**
   * This function will normalize the coordinates
   */
  void normalize(void)
  {
    float scale = 1.0f / getLength();

    x *= scale;
    y *= scale;
    z *= scale;

    return;
  }

  /**
   * This method will calculate a dot product of this
   * vector with another vector
   */
  float dot(const SVector3D & rhs) const
  {
    return (x * rhs.x + y * rhs.y + z * rhs.z);
  }

  const float & operator[](int i) const
  {
    return *((float *) (&x + i));
  }

  /**
   * This member function will multiply vector with constant
   * and assing thre result to it
   */
  SVector3D & operator*=(float rhs)
  {
    x *= rhs;
    y *= rhs;
    z *= rhs;

    return *this;
  }

  /**
   * This member function will subtract vector from this vector
   * and assing thre result to it
   */
  SVector3D & operator-=(const SVector3D & rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;

    return *this;
  }

  /**
   * This function will add two vectors
   */
  friend SVector3D operator+(const SVector3D & lhs, const SVector3D & rhs)
  {
    return SVector3D(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
  }

  /**
   * This function will subtract two vectors
   */
  friend SVector3D operator-(const SVector3D & lhs, const SVector3D & rhs)
  {
    return SVector3D(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
  }

  /**
   * This function will multiply a vector with constant
   */
  friend SVector3D operator*(const SVector3D & lhs, float rhs)
  {
    return SVector3D(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
  }

  /**
   * This function will output vector information to QDebug stream
   */
  friend QDebug & operator<<(QDebug & debug, const SVector3D & vec);
};

} // End of namespace Maths

#endif
