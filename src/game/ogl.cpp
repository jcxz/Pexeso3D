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
 * Implements several utility functions
 */

#include "ogl.h"


namespace OpenGL {

/* intialize a static instance of extensions */
SExtensions SExtensions::instance;


/**
 */
bool SExtensions::initExtensions(const QGLContext *context)
{
  bool ok = true;

  #define RESOLVE_GL_FUNC(f) \
    ok = ok && bool((f = (_gl##f) context->getProcAddress("gl" #f)))

  RESOLVE_GL_FUNC(GenBuffers);
  RESOLVE_GL_FUNC(BindBuffer);
  RESOLVE_GL_FUNC(BufferData);
  RESOLVE_GL_FUNC(DeleteBuffers);
  RESOLVE_GL_FUNC(MapBuffer);
  RESOLVE_GL_FUNC(UnmapBuffer);

  #undef RESOLVE_GL_FUNC

  return ok;
}


/**
 */
const char *glErrorToString(GLenum errcode)
{
  static const struct {
    GLenum code;
    const char *str;
  } gl_errors[] = {
    { GL_NO_ERROR,                      "GL_NO_ERROR" },
    { GL_INVALID_ENUM,                  "GL_INVALID_ENUM" },
    { GL_INVALID_VALUE,                 "GL_INVALID_VALUE" },
    { GL_INVALID_OPERATION,             "GL_INVALID_OPERATION" },
    { GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION" },
    { GL_OUT_OF_MEMORY,                 "GL_OUT_OF_MEMORY" },
    { GL_STACK_UNDERFLOW,               "GL_STACK_UNDERFLOW" },
    { GL_STACK_OVERFLOW,                "GL_STACK_OVERFLOW" },
  };

  /* find the corresponding error message */
  for (unsigned i = 0; i < (sizeof(gl_errors) / sizeof(*gl_errors)); ++i)
  {
    if (errcode == gl_errors[i].code)
    {
      return gl_errors[i].str;
    }
  }

  return "";
}

} // end of namespace
