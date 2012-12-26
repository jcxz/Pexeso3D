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
 * OpenGL include header
 */

#ifndef OGL_H
#define OGL_H

#include <QtOpenGL>

//#define GLEW_STATIC   // force static linkage of GLEW
//#include <GL/glew.h>
//#include <GL/gl.h>  // this shouldn't be necessary after GLEW is included
#include <GL/glu.h>

/* APIENTRY definition */
#ifndef Q_WS_MAC
# ifndef APIENTRYP
#  ifdef APIENTRY
#   define APIENTRYP APIENTRY *
#  else
#   define APIENTRY
#   define APIENTRYP *
#  endif
# endif
#else
# define APIENTRY
# define APIENTRYP *
#endif

/* a workaround because this macro was originally an extension,
   maybe GL_MULTISAMPLE_ARB will yet work except of this */
#ifndef GL_MULTISAMPLE
# define GL_MULTISAMPLE  0x809D
#endif

/* a workaround, this is probabaly an extension also */
#ifndef GL_BGRA
# define GL_BGRA 0x80E1
#endif

/* defines from OpenGL 1.5 */
#ifndef GL_VERSION_1_5
# define GL_ARRAY_BUFFER 0x8892
# define GL_ELEMENT_ARRAY_BUFFER 0x8893
# define GL_READ_WRITE 0x88BA
# define GL_WRITE_ONLY 0x88B9
# define GL_STATIC_DRAW 0x88E4
#endif

#ifndef GL_ARB_vertex_buffer_object
//typedef ptrdiff_t GLsizeiptrARB;
typedef ptrdiff_t GLsizeiptr;
#endif

/* typedef for extensions functions */
typedef void (APIENTRY *_glGenBuffers) (GLsizei, GLuint *);
typedef void (APIENTRY *_glBindBuffer) (GLenum, GLuint);
//typedef void (APIENTRY *_glBufferData) (GLenum, GLsizeiptrARB, const GLvoid *, GLenum);
typedef void (APIENTRY *_glBufferData) (GLenum, GLsizeiptr, const GLvoid *, GLenum);
typedef void (APIENTRY *_glDeleteBuffers) (GLsizei, const GLuint *);
typedef void *(APIENTRY *_glMapBuffer) (GLenum, GLenum);
typedef GLboolean (APIENTRY *_glUnmapBuffer) (GLenum);


/**
 * This namespace defines OpenGL related stuff
 */
namespace OpenGL {

  /**
   * This class defines a collection of extensions
   * required by this program
   */
  struct SExtensions
  {
    _glGenBuffers GenBuffers;
    _glBindBuffer BindBuffer;
    _glBufferData BufferData;
    _glDeleteBuffers DeleteBuffers;
    _glMapBuffer MapBuffer;
    _glUnmapBuffer UnmapBuffer;

    static SExtensions instance;   /// a global object to access extensions

    /**
     * This method will intialize OpenGL Extensions using
     * a given OpenGL context
     *
     * @param context a context to be used
     *
     * @return true if all extensions could be initialised, false otherwise
     */
    bool initExtensions(const QGLContext *context);

    private:
      /**
       * Constructor
       */
      SExtensions(void)
        : GenBuffers(0),
          BindBuffer(0),
          BufferData(0),
          DeleteBuffers(0),
          MapBuffer(0)
      {
      }
  };

  /**
   * This function will translate an OpenGL error code to string
   *
   * @param errcode an OpenGL error code
   *
   * @return a statically allocated error string, or an empty string
   *         when no translation was found
   */
  const char *glErrorToString(GLenum errcode);
}

/* macros to access extensions functions */
#define glGenBuffers OpenGL::SExtensions::instance.GenBuffers
#define glBindBuffer OpenGL::SExtensions::instance.BindBuffer
#define glBufferData OpenGL::SExtensions::instance.BufferData
#define glDeleteBuffers OpenGL::SExtensions::instance.DeleteBuffers
#define glMapBuffer OpenGL::SExtensions::instance.MapBuffer
#define glUnmapBuffer OpenGL::SExtensions::instance.UnmapBuffer

#endif
