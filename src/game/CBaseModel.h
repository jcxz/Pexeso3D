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
 * This file defines the model base class
 */

#ifndef CBASEMODEL_H
#define CBASEMODEL_H

#include "CIndexBuffer.h"
#include "CVertexBuffer.h"
#include "CTextureStore.h"
#include "CBBox.h"



/* forward declarations */
class QDebug;

/**
 * This class represents an abstract base for all in game models
 */
class CBaseModel
{
  public:
    /**
     * Constructor
     */
    CBaseModel(void)
      : m_vb(),
        m_ib(),
        m_tex_ind(CTextureStore::INVALID_INDEX),
        m_id(0),
        m_bbox(Maths::SVector3D(0.0f, 0.0f, 0.0f), 0.0f)
    {
      m_state.covered  = true;
      m_state.selected = false;
      m_state.deleted  = false;
    }

    /**
     * Destructor
     */
    virtual ~CBaseModel(void)
    {
    }

    /**
     * This method will return a buffer with vertexes
     */
    const CVertexBuffer & getVertexBuffer(void) const
    {
      return m_vb;
    }

    /**
     * This method will return a buffer with indices
     */
    const CIndexBuffer & getIndexBuffer(void) const
    {
      return m_ib;
    }

    /**
     * To get the texture index
     */
    int getTextureIndex(void) const
    {
      return m_tex_ind;
    }

    /**
     * To set the texture index
     */
    void setTextureIndex(int index)
    {
      m_tex_ind = index;
      return;
    }

    /**
     * Get the object's selection id
     */
    GLint getID(void) const
    {
      return m_id;
    }

    /**
     * Set the object's selection id
     */
    void setID(GLint id)
    {
      m_id = id;
      return;
    }

    /**
     * Convenience function to determine if the model is selected
     */
    bool isSelected(void) const
    {
      return m_state.selected;
    }

    /**
     * Make this model selected
     */
    void setSelected(bool val = true)
    {
      m_state.selected = val;
      return;
    }

    /**
     * Determines of the model is to be covered or uncovered
     */
    bool isCovered(void) const
    {
      return m_state.covered;
    }

    /**
     * Toggle whether the model is covered or uncovered
     */
    void setCovered(bool val = true)
    {
      m_state.covered = val;
      return;
    }

    /**
     * Determines if the model was guessed by player and
     * thus removed from scene
     */
    bool isDeleted(void)
    {
      return m_state.deleted;
    }

    /**
     * Mark this model as deleted
     */
    void setDeleted(bool val = true)
    {
      m_state.deleted = val;
      return;
    }

    /**
     * This method will get the bounding box to do
     * intersection calculations
     */
    const CBBox & getBBox(void) const
    {
      return m_bbox;
    }

    /**
     * This method will set the bounding box
     */
    void setBBox(const CBBox & bbox)
    {
      m_bbox = bbox;
      return;
    }

    /**
     * This method will output debug information to QDebug stream
     */
    friend QDebug & operator<<(QDebug & debug, const CBaseModel & model);

  protected:
    CVertexBuffer m_vb;  /// vertex buffer to store model's geometry
    CIndexBuffer m_ib;   /// index buffer to store model's geometry indices
    int m_tex_ind;       /// index of texture in texture store

  private:
    struct {
      unsigned char covered  : 1;  /// whether the model was covered
      unsigned char selected : 1;  /// whether the model was selected
      unsigned char deleted  : 1;  /// whether the model was deleted
    } m_state;                     /// the state of model
    GLint m_id;                    /// object's id as used for selections and stenciling
    CBBox m_bbox;                  /// model's bounding box information
};

#endif
