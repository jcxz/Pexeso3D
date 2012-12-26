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
 * Definition of a 2D vector
 */

#ifndef SVECTOR2D_H
#define SVECTOR2D_H


class QDebug;

namespace Maths {

/**
 * Two dimensional vector
 */
struct SVector2D
{
  float x;  /// x coordinate
  float y;  /// y coordinate

  /**
   * This constructor does absolutely nothing
   * to avoid any initialisation overhead when not needed
   */
  SVector2D(void)
  {
  }

  /**
   * Constructor
   *
   * @param x the x coordinate position
   * @param y the y coordinate position
   */
  SVector2D(float x, float y)
    : x(x), y(y)
  {
  }

  /**
   * This function will output vector information to QDebug stream
   */
  friend QDebug & operator<<(QDebug & debug, const SVector2D & vec);
};

} // End of namespace Maths

#endif
