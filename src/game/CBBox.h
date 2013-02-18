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
 * Definition a bounding box of a model or a scene
 */

#ifndef CBBOX_H
#define CBBOX_H

#include <QDebug>

#include "Maths.h"


/**
 * This class represents an axis aligned bounding box
 * (which means that the box is not rotated in any way
 * about origin)
 * i.e a minimal box that can still fit in the model (or the scene)
 */
class CBBox
{
  public:
    CBBox(void)
      : m_min(0.0f, 0.0f, 0.0f),
        m_max(0.0f, 0.0f, 0.0f)
    { }

    CBBox(const Maths::SVector3D & pos, float side)
      : m_min(pos),
        m_max(pos.x + side, pos.y + side, pos.z + side)
    { }

    /**
     * This method will return the minimum bound of bounding box
     */
    const Maths::SVector3D & getMin(void) const
    {
      return m_min;
    }

    /**
     * This method will return the greater bound of bounding box
     */
    const Maths::SVector3D & getMax(void) const
    {
      return m_max;
    }

    /**
     * This method will compute the center of bounding box
     *
     * @return a positon in the center of nounding box
     */
    Maths::SVector3D CBBox::getCenter(void) const
    {
      return Maths::SVector3D((m_max.x + m_min.x) / 2.0f,
                              (m_max.y + m_min.y) / 2.0f,
                              (m_max.z + m_min.z) / 2.0f);
    }

    /**
     * Tato funkcia bude konvertovat axis-aligned bounding box
     * na oriented bounding box, tak ze najprv urci vektory
     * u, v, w (to budu strany tej bbox) a potom tieto vektory
     * transformuje touto transformacnou maticou.
     * Tuna sa bude predavat transformacna matica (cize world matrix),
     * cize ta kde budu nazhromazdene vsetky transformacie a rotacie sveta
     */
    //COBBox toOBBox(const SMatrix4D & transform);

    /**
     * This method will reset the bounding box to zero dimensions
     */
    void reset(void);

    /**
     * Expand to fit a new object. Note that this method
     * will never cause the bounding box to shrink.
     *
     * @param bbox the bounding box to be adjusted to
     */
    void adjust(const CBBox & bbox);

    /**
     * Adjust the bounding box to include this point
     *
     * @param pt a point to be included
     */
    void adjust(const Maths::SVector3D & pt);

    /**
     * Compute the intersection of ray with this bounding box
     *
     * @param origin origin of the ray
     * @param dir direction of the ray
     *
     * @return true if the ray intersects with this bounding box,
     *         false otherwise
     */
    bool intersects(const Maths::SVector3D & origin,
                    const Maths::SVector3D & dir,
                    float *tmin,
                    float *tmax) const;

    /**
     * A function to output bounding box information to QDebug stream
     */
    friend QDebug & operator<<(QDebug & debug, const CBBox & bbox);

  private:
    Maths::SVector3D m_min;  /// left bottom back corner of bbox
    Maths::SVector3D m_max;  /// right top front corner of bbox
};

#endif
