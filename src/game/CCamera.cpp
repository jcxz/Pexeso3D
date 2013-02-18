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
 * Camera class implementation
 */

#include "CCamera.h"
#include "Global.h"
#include "ogl.h"



/**
 */
Maths::SVector3D CCamera::unproject(int x, int y) const
{
  GLint viewport[4];             // viewport (i.e windows size)
  GLdouble modelview[16];        // modelview matrix
  GLdouble projection[16];       // projection matrix
  GLfloat win_x, win_y, win_z;   // window coordinates
  GLdouble pos_x, pos_y, pos_z;  // eye space coordinates

  /* get matrices */
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetIntegerv(GL_VIEWPORT, viewport);

  /* convert from window coordinates to opengl coordinates and get z value */
  win_x = (float) x;
  win_y = (float) viewport[3] - (float)y;

  // this value can be probably arbitrary (but within (0, 1], say 0.5)
  // and I probably dont need to mess with framebuffer
  glReadPixels((GLint) win_x, (GLint) win_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);

  /* get the eye space coordinates */
  gluUnProject(win_x, win_y, win_z,
               modelview, projection, viewport,
               &pos_x, &pos_y, &pos_z);

  return Maths::SVector3D((GLfloat) pos_x, (GLfloat) pos_y, (GLfloat) pos_z);
}


/**
 */
void CCamera::buildCameraRay(int x, int y, Maths::SVector3D *origin, Maths::SVector3D *dir) const
{
  PEXESO_ASSERT(origin != 0);
  PEXESO_ASSERT(dir != 0);

  GLint viewport[4];             // viewport (i.e windows size)
  GLdouble modelview[16];        // modelview matrix
  GLdouble projection[16];       // projection matrix
  GLfloat win_x, win_y, win_z;   // window coordinates
  GLdouble pos_x, pos_y, pos_z;  // eye space coordinates

  /* get matrices */
  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  glGetDoublev(GL_PROJECTION_MATRIX, projection);
  glGetIntegerv(GL_VIEWPORT, viewport);

  /* convert from window coordinates to opengl coordinates and get z value */
  win_x = (float) x;
  win_y = (float) viewport[3] - (float)y;

  // this value can be probably arbitrary (but within (0, 1], say 0.5)
  // and I probably dont need to mess with framebuffer
  glReadPixels((GLint) win_x, (GLint) win_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &win_z);

  /* get the eye space coordinates */
  gluUnProject(win_x, win_y, win_z,
               modelview, projection, viewport,
               &pos_x, &pos_y, &pos_z);

  /* build ray origin */
  origin->x = (GLfloat) modelview[12];  // x translation element of modelview matrix
  origin->y = (GLfloat) modelview[13];  // y translation element of modelview matrix
  origin->z = (GLfloat) modelview[14];  // z translation element of modelview matrix

  //origin->x = (GLfloat) modelview[3];  // x translation element of modelview matrix
  //origin->y = (GLfloat) modelview[7];  // y translation element of modelview matrix
  //origin->z = (GLfloat) modelview[11];  // z translation element of modelview matrix

  /* build ray direction */
  dir->x = (GLfloat) (pos_x - origin->x);
  dir->y = (GLfloat) (pos_y - origin->y);
  dir->z = (GLfloat) (pos_z - origin->z);

  // maybe normalize direction here
  dir->normalize();

  return;
}
