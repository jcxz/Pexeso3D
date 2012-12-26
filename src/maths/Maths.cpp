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
 * Implementation of some the helper mathematical functions
 */

#include <QDebug>

#include "Maths.h"


namespace Maths {

/**
 */
QDebug & operator<<(QDebug & debug, const SVector2D & vec)
{
  debug.nospace() << "SVector2D(" << vec.x << ", " << vec.y << ")";
  return debug.maybeSpace();
}


/**
 */
QDebug & operator<<(QDebug & debug, const SVector3D & vec)
{
  debug.nospace() << "SVector3D(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
  return debug.maybeSpace();
}


/**
 */
QDebug & operator<<(QDebug & debug, const SVertex & vert)
{
  debug.nospace() << "SVertex(" << vert.pos << ", " << vert.tex << ")";
  return debug.maybeSpace();
}

} // End of namespace Maths
