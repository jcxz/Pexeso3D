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
 * This file declares some usefull mathematical functions and helper structures
 */

#ifndef MATHS_H
#define MATHS_H

#include <cmath>

#include "SVector2D.h"
#include "SVector3D.h"


namespace Maths {

/**
 * A model vertex
 */
struct SVertex
{
  SVector3D pos;  /// object coordinates
  SVector2D tex;  /// texture coordinates

  /**
   * Constructor
   */
  SVertex(void)
  {
  }

  /**
   * Constructor
   *
   * @param pos the position coordinates
   * @param tex the texture coordinates
   */
  SVertex(const SVector3D & pos, const SVector2D & tex)
    : pos(pos), tex(tex)
  {
  }

  /**
   * This function will output vertex information to QDebug stream
   */
  friend QDebug & operator<<(QDebug & debug, const SVertex & vert);
};


/**
 * This structure represents a 4x4 transformation matrix
 */
struct SMatrix4D
{
  /* union {
    struct {  // this is prohibited by ISO C++98   // ???
      float d11, d21, d31, d41;
      float d12, d22, d32, d42;
      float d13, d23, d33, d43;
      float d14, d24, d34, d44;
    };
    float data[16];
  };
  */

  SMatrix4D(void) { }

  SMatrix4D(const float *data);

  void setTransl(float x, float y, float z);
  void setRot(float angle, float x, float y, float z);
  void setScale(float x, float y, float z);

  friend SMatrix4D operator*(const SMatrix4D & lhs, const SMatrix4D & rhs);
  friend SMatrix4D operator*(const SVector3D & lhs, const SMatrix4D & rhs);
};

} // End of namespace Maths

#endif
