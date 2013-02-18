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
 * Texture implementation
 */

#include <QDebug>
#include <QImage>

#include "CTextureStore.h"



/**
 */
void CTextureStore::clear(void)
{
  for (CIDListIter it = m_tex_ids.begin(); it != m_tex_ids.end(); ++it)
  {
    glDeleteTextures(1, &(*it));
  }

  m_tex_ids.clear();

  return;
}


/**
 */
int CTextureStore::loadTexture(const QString & filename, int index)
{
  /* load the image and convert it to appropriate format */
  QImage image(filename);

  if (image.isNull())
  {
    qDebug() << "Failed to load texture: " << filename;
    return INVALID_INDEX;
  }

  image = image.convertToFormat(QImage::Format_ARGB32);

  /* cancel previous error flags */
  glGetError();

  /* try to store the texture on GPU */
  GLuint tex_id = 0;
  glGenTextures(1, &tex_id);             // generate a new texture id
  glBindTexture(GL_TEXTURE_2D, tex_id);  // bind the generated texture
  glTexImage2D(GL_TEXTURE_2D,            // the type of texture
               0,                        // mipmapping level
               GL_RGBA,                  // pixel format of the texture in GPU
               image.width(),            // image width
               image.height(),           // image height
               0,                        // according to documentation this value must be 0
               GL_BGRA,                  // pixel format of pixels in the input QImage
               GL_UNSIGNED_BYTE,         // the data type of array in the last argument
               image.constBits());       // the actual pixels to be copied

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glBindTexture(GL_TEXTURE_2D, 0);

  /* check on opengl errors */
  GLenum error = glGetError();
  if (error != GL_NO_ERROR)
  {
    qDebug() << "Failed to load texture: " << OpenGL::glErrorToString(error);
    return INVALID_INDEX;
  }

  /* erase the old texture on reload */
  if ((index >= 0) && (index < m_tex_ids.size()))
  {
    GLuint old_id = m_tex_ids[index];
    if (old_id != 0)
    {
      glDeleteTextures(1, &old_id);
    }
    m_tex_ids[index] = tex_id;

    return index;
  }

  m_tex_ids.append(tex_id);

  return m_tex_ids.size() - 1;
}
