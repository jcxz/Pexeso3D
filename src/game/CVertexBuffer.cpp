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
 * Vertex buffer implementation
 */

#include <QDebug>

#include "CVertexBuffer.h"



/**
 */
bool CVertexBuffer::bufferData(const Maths::SVertex *data, GLsizeiptr len, GLenum usage)
{
  /* reset error flag */
  glGetError();

  /* delete previous contents */
  if (m_id != 0)
  {
    glDeleteBuffers(1, &m_id);
  }

  /* create a new buffer and load it with data */
  m_len = len;
  glGenBuffers(1, &m_id);                     // generate new buffer id
  glBindBuffer(GL_ARRAY_BUFFER, m_id);        // bind the generated  buffer
  glBufferData(GL_ARRAY_BUFFER,               // the type of buffer that is being loaded
               len * sizeof(Maths::SVertex),  // the size of the array that will be loaded (in bytes)
               data,                          // the actual array
               usage);                        // usage mode
  glBindBuffer(GL_ARRAY_BUFFER, 0);           // cancel the buffer binding

  /* check on errors */
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    qDebug() << "Failed to load vertex buffer: " << OpenGL::glErrorToString(error);
    return false;
  }

  return true;
}
