/*
 * Copyright (C) 2012-2013 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
 * Copyright (C) 2011 Wildfire Games.
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
 */

/*
 * The code in this file was taken from 0.a.d opensource
 * strategy game. Though it is not currently used in our game,
 * you can find it at this website:
 * http://www.wildfiregames.com/0ad/
 */

#ifndef COBBOX_H
#define COBBOX_H

#include "Maths.h"


class CBBox;

/**
 * This class represents an oriented bounding box
 */
class COBBox
{
  public:

    /// Empty constructor; creates an empty box
    COBBox(void)
    {
      setEmpty();
    }

    /**
     * Constructs a new oriented box centered at @p center and with normalized side vectors @p u, @p v and @p w. These vectors should
     * be mutually orthonormal for a proper rectangular box. The half-widths of the box in each dimension are given by the corresponding
     * components of @p halfSizes.
     */
    COBBox(const Maths::SVector3D & center,
           const Maths::SVector3D & u,
           const Maths::SVector3D & v,
           const Maths::SVector3D & w,
           const Maths::SVector3D & halfSizes)
     : m_Center(center),
       m_HalfSizes(halfSizes)
    {
      m_Basis[0] = u;
      m_Basis[1] = v;
      m_Basis[2] = w;
    }

    /// Constructs a new box from an axis-aligned bounding box (AABB).
    explicit COBBox(const CBBox & bbox);

    /**
     * Returns true if the ray originating in @p origin and with unit direction vector @p dir intersects this box, false otherwise.
     * Additionally, returns the distance in the positive direction from the origin of the ray to the entry and exit points in the
     * box in @p tMin and @p tMax. If the origin is inside the box, then this is counted as an intersection and one of @p tMin and
     * @p tMax may be negative.
     *
     * Should not be used if IsEmpty() is true.
     * See also Real-Time Rendering, Third Edition by T. Akenine-Möller, p. 741--742.
     *
     *
     * @param origin Origin of the ray.
     * @param dir Direction vector of the ray, defining the positive direction of the ray. Must be of unit length.
     */
    bool intersects(const Maths::SVector3D & origin,
                    const Maths::SVector3D & dir,
                    float *tmin_out,
                    float *tmax_out) const;

    /**
     * Returns the corner at coordinate (@p u, @p v, @p w). Each of @p u, @p v and @p w must be exactly 1 or -1.
     * Should not be used if IsEmpty() is true.
     */
    void getCorner(int u, int v, int w, Maths::SVector3D & out) const
    {
      out = m_Center + m_Basis[0] * (u * m_HalfSizes.x)
                     + m_Basis[1] * (v * m_HalfSizes.y)
                     + m_Basis[2] * (w * m_HalfSizes.z);
    }

    void setEmpty(void)
    {
      // everything is zero
      m_Center = Maths::SVector3D();
      m_Basis[0] = Maths::SVector3D();
      m_Basis[1] = Maths::SVector3D();
      m_Basis[2] = Maths::SVector3D();
      m_HalfSizes = Maths::SVector3D();
    }

    /*
    bool IsEmpty() const
    {
      SVector3D empty(0.0f, 0.0f, 0.0f);
      return (m_Center == empty &&
              m_Basis[0] == empty &&
              m_Basis[1] == empty &&
              m_Basis[2] == empty &&
              m_HalfSizes == empty);
    }
    */

  public:
    Maths::SVector3D m_Center;    ///< Centroid location of the box
    Maths::SVector3D m_HalfSizes; ///< Half the sizes of the box in each dimension (u,v,w). Positive values are expected.
      /// Basis vectors (u,v,w) of the sides. Must always be normalized, and should be
      /// orthogonal for a proper rectangular cuboid.
    Maths::SVector3D m_Basis[3];

    static const COBBox EMPTY;
};

#endif
