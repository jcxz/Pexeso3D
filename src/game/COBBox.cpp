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
 * Implementation of the oriented bounding box
 */

/*
 * The code in this file was taken from 0.a.d opensource
 * strategy game. Though it is not currently used in our game,
 * you can find it at this website:
 * http://www.wildfiregames.com/0ad/
 */

#include "COBBox.h"
#include "CBBox.h"

#include <float.h>



const COBBox COBBox::EMPTY = COBBox();



COBBox::COBBox(const CBBox & bbox)
{
  m_Center = bbox.getCenter();

  // the axes of an AABB are the world-space axes
  m_Basis[0].x = 1.f; m_Basis[0].y = 0.f; m_Basis[0].z = 0.f;
  m_Basis[1].x = 0.f; m_Basis[1].y = 1.f; m_Basis[1].z = 0.f;
  m_Basis[2].x = 0.f; m_Basis[2].y = 0.f; m_Basis[2].z = 1.f;

  m_HalfSizes = (bbox.getMax() - bbox.getMin()) * 0.5f;

#if 0
    if (bound.IsEmpty())
    {
        SetEmpty();
    }
    else
    {
        bound.GetCentre(m_Center);

        // the axes of an AABB are the world-space axes
        m_Basis[0].X = 1.f; m_Basis[0].Y = 0.f; m_Basis[0].Z = 0.f;
        m_Basis[1].X = 0.f; m_Basis[1].Y = 1.f; m_Basis[1].Z = 0.f;
        m_Basis[2].X = 0.f; m_Basis[2].Y = 0.f; m_Basis[2].Z = 1.f;

        // element-wise division by two to get half sizes (remember, [1] and [0] are the max and min coord points)
        m_HalfSizes = (bound[1] - bound[0]) * 0.5f;
    }
#endif
}


bool COBBox::intersects(const Maths::SVector3D & origin,
                        const Maths::SVector3D & dir,
                        float *tmin_out,
                        float *tmax_out) const
{
  // See Real-Time Rendering, Third Edition, p. 743
  float tMin = -FLT_MAX;
  float tMax = FLT_MAX;
  Maths::SVector3D p = m_Center - origin;

  for (int i = 0; i < 3; ++i)
  {
    float e = m_Basis[i].dot(p);
    float f = m_Basis[i].dot(dir);

    if (fabs(f) > 1e-10f)
    {
      float invF = 1.f/f;
      float t1 = (e + m_HalfSizes[i]) * invF;
      float t2 = (e - m_HalfSizes[i]) * invF;

      if (t1 > t2)
      {
        float tmp = t1;
        t1 = t2;
        t2 = tmp;
      }

      if (t1 > tMin) tMin = t1;
      if (t2 < tMax) tMax = t2;
      if (tMin > tMax) return false;
      if (tMax < 0) return false;
    }
    else
    {
      if (-e - m_HalfSizes[i] > 0 || -e + m_HalfSizes[i] < 0) return false;
    }
  }

  *tmin_out = tMin;
  *tmax_out = tMax;

  return true;
}
