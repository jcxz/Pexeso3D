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
 * Definition of a texture container class
 */

#ifndef CTEXTURESTORE_H
#define CTEXTURESTORE_H

#include <QList>

#include "Global.h"
#include "ogl.h"


/**
 * This class represetns a contained for
 * OpenGL texture id-s
 */
class CTextureStore
{
  public:
    /** a constant defining incorrect index value */
    enum { INVALID_INDEX = -1 };

  public:
    /**
     */
    CTextureStore(void)
      : m_tex_ids()
    {
    }

    /**
     */
    ~CTextureStore(void)
    {
      clear();
    }

    /**
     * This method will free all the textures from texture store
     */
    void clear(void);

    /**
     * This method will return the texture store's size
     */
    int getSize(void) const
    {
      return m_tex_ids.size();
    }

    /**
     * This method will cause that the texture at a given index
     * will be bound. You do not have to unbind the currently bound
     * texture, to bind another one.
     *
     * @param index a texture to be bound
     */
    void bindTexture(int index) const
    {
      if ((index >= 0) && (index < m_tex_ids.size()))
      {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_tex_ids[index]);
      }

      return;
    }

    /**
     * This method will cause that no texture will be bound
     */
    void unbindTexture(void) const
    {
      glBindTexture(GL_TEXTURE_2D, 0);
      glDisable(GL_TEXTURE_2D);
      return;
    }

    /**
     * This method will load/reload the texture from file
     * and return its index in texture store
     */
    int loadTexture(const QString & filename, int index = INVALID_INDEX);

  private:
    /** an iterator for texture id list */
    typedef QList<GLuint>::iterator CIDListIter;

  private:
    QList<GLuint> m_tex_ids;
};

#endif
