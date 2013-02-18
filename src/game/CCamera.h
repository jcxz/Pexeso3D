/*
 * Copyright (C) 2012-2013 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
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
 */

#ifndef CCAMERA_H
#define CCAMERA_H

#include "Maths.h"

/* forward declarations */
struct Maths::SVector3D;

/**
 * This class will hold (or manage) projection
 * and view matrices. It will handle scene rotations,
 * translations and zooming and provide several coveniece
 * methods like building the ray from camera to point of click
 * and setting up projection
 */
class CCamera
{
  public:
    CCamera(void)
    {
    }

    void setPerspectiveProjection(/*...*/);

    /**
     * This method will unproject a mouse screen position to 3D point
     * This function is mostly for debugging
     */
    Maths::SVector3D unproject(int x, int y) const;

    /**
     * x, y are positions of mouse click on screen
     */
    void buildCameraRay(int x, int y, Maths::SVector3D *origin, Maths::SVector3D *dir) const;

  private:
    //SMatrix3D m_view_mat;  /// mozno nebude treba
                           /// a bude queryovat OpenGL  <- ???
};

#endif
