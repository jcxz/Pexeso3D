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
 * This header file defines an OpenGL vertex buffer abstraction
 */

#ifndef CVERTEXBUFFER_H
#define CVERTEXBUFFER_H

#include "Global.h"
#include "ogl.h"
#include "Maths.h"



/**
 * This class wraps up the vertex buffer object
 */
class CVertexBuffer
{
  public:
    /**
     * Constructor
     */
    CVertexBuffer(void)
      : m_len(0),
        m_id(0)
    {
    }

    /**
     * Destructor
     */
    ~CVertexBuffer(void)
    {
      if (m_id != 0)
      {
        glDeleteBuffers(1, &m_id);
      }
    }

    /**
     * This method will return the buffer length
     */
    GLsizeiptr getLength(void) const
    {
      return m_len;
    }

    /**
     * To make this vertex buffer the current buffer for rendering
     */
    void bind(void) const
    {
      glBindBuffer(GL_ARRAY_BUFFER, m_id);
      glEnableClientState(GL_VERTEX_ARRAY);
      glEnableClientState(GL_TEXTURE_COORD_ARRAY);
      glTexCoordPointer(2,                                           // there are 2 texture axes (x, y)
                        GL_FLOAT,                                    // they are float coordinates
                        sizeof(Maths::SVertex),                      // stride (every sizeof(SVertex) bytes is one texture coordinate)
                        PEXESO_MEMBER_OFFSET(Maths::SVertex, tex));  // a pointer to the first coordinate (meassured in floats)

      glVertexPointer(3,                                          // vertex position has three coordinates (x, y, z)
                      GL_FLOAT,                                   // they are of float type
                      sizeof(Maths::SVertex),                     // they sizeof(SVertex) bytes each
                      PEXESO_MEMBER_OFFSET(Maths::SVertex, pos)); // and they start on offset given by pos member (meassured in floats)
      return;
    }

    /**
     * To cancel this vertex buffer from being current rendering buffer
     */
    void unbind(void) const
    {
      glDisableClientState(GL_TEXTURE_COORD_ARRAY);
      glDisableClientState(GL_VERTEX_ARRAY);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      return;
    }


    /**
     * This method allows to access the data from buffer allocated on GPU.
     * Note that no rendering may be performed while the buffer is locked.
     *
     * @param how the data will be accessed, the default mode is to read/write
     *        refer to OpenGL documentation for glMapBuffer() call
     *
     * @return a mapped pointer to the data from GPU, NULL on error
     */
    Maths::SVertex *lock(GLenum access = GL_READ_WRITE) const
    {
      glBindBuffer(GL_ARRAY_BUFFER, m_id);
      Maths::SVertex *data = (Maths::SVertex *) glMapBuffer(GL_ARRAY_BUFFER, access);
      glBindBuffer(GL_ARRAY_BUFFER, 0);  // according to documentation this should be safe
      return data;
    }

    /**
     * This method will cancel the lock of the buffer
     */
    void unlock(void) const
    {
      glBindBuffer(GL_ARRAY_BUFFER, m_id);
      glUnmapBuffer(GL_ARRAY_BUFFER);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      return;
    }

    /**
     * This method will load the vertex buffer with bytes from *data
     * and store it on GPU. If *data is NULL only a size amount
     * of bytes will be allocated on GPU. This allocated storage
     * can be accessed later by calling lock() function.
     *
     * @param data a pointer to data in system memory that will be copied
     *        to GPU memory
     * @param len the size of data to be copied (in counts of SVertex-s)
     * @param usage how the data will be used, refer to OpenGL documentation
     *        for this parameter
     *
     * @return true on success, false on error
     */
    bool bufferData(const Maths::SVertex *data, GLsizeiptr len, GLenum usage = GL_STATIC_DRAW);

  private:
    GLsizeiptr m_len;  /// vertex buffer length
    GLuint m_id;       /// vertex buffer id
};

#endif
