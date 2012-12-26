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
 * This file contains implementation of some of the index buffer's methods
 */

#include <QDebug>

#include "CIndexBuffer.h"



/**
 */
bool CIndexBuffer::bufferData(const GLuint *data, GLsizeiptr len, GLenum usage)
{
  /* reset possible previous error flags */
  glGetError();

  /* delete the previous buffer if any */
  if (m_id != 0)
  {
    glDeleteBuffers(1, &m_id);
  }

  /* create a new buffer */
  m_len = len;
  glGenBuffers(1, &m_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,        // the type of buffer
               len * sizeof(GLuint),           // the size of index buffer in bytes
               data,                           // the buffer to be stored
               usage);                         // usage type
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  /* check on errors */
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    qDebug() << "Failed to load vertex buffer: " << OpenGL::glErrorToString(error);
    return false;
  }

  return true;
}
