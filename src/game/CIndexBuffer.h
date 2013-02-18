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
 * This header file defines an OpenGL index buffer abstraction
 */

#ifndef CINDEXBUFFER_H
#define CINDEXBUFFER_H

#include "ogl.h"


/**
 */
class CIndexBuffer
{
  public:
    /**
     * Constructor
     */
    CIndexBuffer(void)
      : m_len(0),
        m_id(0)
    {
    }

    /**
     * Destructor
     */
    ~CIndexBuffer(void)
    {
      if (m_id != 0)
      {
        glDeleteBuffers(1, &m_id);
      }
    }

    /**
     * This function will return the buffer length
     */
    GLsizeiptr getLength(void) const
    //GLsizeiptrARB getLength(void) const
    {
      return m_len;
    }

    /**
     * The type of elements in the buffer
     */
    GLenum getType(void) const
    {
      return GL_UNSIGNED_INT;
    }

    /**
     * Makes this index buffer, the current index buffer
     */
    void bind(void) const
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
      return;
    }

    /**
     * Cancels this index buffer from being the current index buffer
     */
    void unbind(void) const
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
    GLuint *lock(GLenum access = GL_READ_WRITE) const
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
      GLuint *data = (GLuint *) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, access);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      return data;
    }

    /**
     * This method will cancel the lock of the buffer
     */
    void unlock(void) const
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
      glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
      return;
    }

    /**
     * This method will load the index buffer with bytes from *data
     * and store it on GPU. If *data is NULL only a size amount
     * of bytes will be allocated on GPU. This allocated storage
     * can be accessed later by calling lock() function.
     *
     * @param data a pointer to data in system memory that will be copied
     *        to GPU memory
     * @param len the size of data to be copied (in counts of GLuint-s)
     * @param usage how the data will be used, refer to OpenGL documentation
     *        for this parameter
     *
     * @return true on success, false on error
     */
    bool bufferData(const GLuint *data, GLsizeiptr len, GLenum usage = GL_STATIC_DRAW);

  private:
    GLsizeiptr m_len;  /// buffer length
    GLuint m_id;       /// buffer id
};

#endif
