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
 * Implemtation of cube model
 */

#include <QDebug>

#include "CCubeModel.h"



/**
 */
bool CCubeModel::build(const Maths::SVector3D & pos, GLfloat side)
{
  /* cube positional coordinates */
  static const Maths::SVector3D cube_pos_coords[] = {
    /* front face */
    Maths::SVector3D( 1.0f,  0.0f, 1.0f),
    Maths::SVector3D( 1.0f,  1.0f, 1.0f),
    Maths::SVector3D( 0.0f,  1.0f, 1.0f),
    Maths::SVector3D( 0.0f,  0.0f, 1.0f),

    /* back face */
    Maths::SVector3D( 0.0f,  0.0f,  0.0f),
    Maths::SVector3D( 0.0f,  1.0f,  0.0f),
    Maths::SVector3D( 1.0f,  1.0f,  0.0f),
    Maths::SVector3D( 1.0f,  0.0f,  0.0f),

    /* top face */
    Maths::SVector3D( 0.0f,  1.0f,  0.0f),
    Maths::SVector3D( 0.0f,  1.0f,  1.0f),
    Maths::SVector3D( 1.0f,  1.0f,  1.0f),
    Maths::SVector3D( 1.0f,  1.0f,  0.0f),

    /* bottom face */
    Maths::SVector3D( 0.0f,  0.0f,  1.0f),
    Maths::SVector3D( 0.0f,  0.0f,  0.0f),
    Maths::SVector3D( 1.0f,  0.0f,  0.0f),
    Maths::SVector3D( 1.0f,  0.0f,  1.0f),

    /* right face */
    Maths::SVector3D( 1.0f,  0.0f,  0.0f),
    Maths::SVector3D( 1.0f,  1.0f,  0.0f),
    Maths::SVector3D( 1.0f,  1.0f,  1.0f),
    Maths::SVector3D( 1.0f,  0.0f,  1.0f),

    /* left face */
    Maths::SVector3D( 0.0f,  0.0f,  1.0f),
    Maths::SVector3D( 0.0f,  1.0f,  1.0f),
    Maths::SVector3D( 0.0f,  1.0f,  0.0f),
    Maths::SVector3D( 0.0f,  0.0f,  0.0f)
  };
  
  /* cube's texture coordinates */
  static const Maths::SVector2D cube_tex_coords[] = {
    /* front face texture */
    Maths::SVector2D(1.0f, 1.0f),
    Maths::SVector2D(1.0f, 0.0f),
    Maths::SVector2D(0.0f, 0.0f),
    Maths::SVector2D(0.0f, 1.0f),

    /* back face texture */
    Maths::SVector2D(1.0f, 1.0f),
    Maths::SVector2D(1.0f, 0.0f),
    Maths::SVector2D(0.0f, 0.0f),
    Maths::SVector2D(0.0f, 1.0f),

    /* top face texture */
    Maths::SVector2D(0.0f, 0.0f),
    Maths::SVector2D(0.0f, 1.0f),
    Maths::SVector2D(1.0f, 1.0f),
    Maths::SVector2D(1.0f, 0.0f),

    /* bottom face texture */
    Maths::SVector2D(0.0f, 0.0f),
    Maths::SVector2D(0.0f, 1.0f),
    Maths::SVector2D(1.0f, 1.0f),
    Maths::SVector2D(1.0f, 0.0f),

    /* right face texture */
    Maths::SVector2D(1.0f, 1.0f),
    Maths::SVector2D(1.0f, 0.0f),
    Maths::SVector2D(0.0f, 0.0f),
    Maths::SVector2D(0.0f, 1.0f),

    /* left face texture */
    Maths::SVector2D(1.0f, 1.0f),
    Maths::SVector2D(1.0f, 0.0f),
    Maths::SVector2D(0.0f, 0.0f),
    Maths::SVector2D(0.0f, 1.0f)
  };

  /* indexes for the cube vertices */
  static const GLuint indices[] = {
     0,  1,  2,  3,
     4,  5,  6,  7,
     8,  9, 10, 11,
    12, 13, 14, 15,
    16, 17, 18, 19,
    20, 21, 22, 23
  };

  /* number of elements in cube pos/tex coords and indices arrays */
  static const unsigned int num_elems = sizeof(indices) / sizeof(*indices);

  /* allocate memory on GPU */
  bool vb_res = m_vb.bufferData(NULL, num_elems);
  bool ib_res = m_ib.bufferData(NULL, num_elems);

  if ((!vb_res) || (!ib_res))
  {
    qDebug() << "Failed to allocate memory in GPU for index or vertex buffer";
    return false;
  }

  /* lock the buffers for writing */
  Maths::SVertex *vptr = m_vb.lock(GL_WRITE_ONLY);
  GLuint *iptr  = m_ib.lock(GL_WRITE_ONLY);

  if ((vptr == NULL) || (iptr == NULL))
  {
    qDebug() << "Failed to lock index or vertex buffer";
    return false;
  }

  /* calculate the cube offset */
  Maths::SVector3D cube_pos(pos.x * side, pos.y * side, pos.z * side);

  /* load the model to GPU */
  for (unsigned int i = 0; i < num_elems; ++i)
  {
    vptr[i] = Maths::SVertex(cube_pos + cube_pos_coords[i] * side, cube_tex_coords[i]);
    iptr[i] = indices[i];
  }

  /* unlock the buffers */
  m_ib.unlock();
  m_vb.unlock();

  /* set the bounding box */
  setBBox(CBBox(cube_pos, side));

  return true;
}
