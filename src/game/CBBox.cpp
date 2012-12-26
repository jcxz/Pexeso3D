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
 * Implementation an axis aligned bounding box
 */

#include <float.h>

#include "CBBox.h"


/**
 */
Maths::SVector3D CBBox::getCenter(void) const
{
  /*
  return Maths::SVector3D((m_pos2.x - m_pos1.x) / 2.0f,
                          (m_pos2.y - m_pos1.y) / 2.0f,
                          (m_pos2.z - m_pos1.z) / 2.0f);
  */

  return Maths::SVector3D((m_pos2.x + m_pos1.x) / 2.0f,
                          (m_pos2.y + m_pos1.y) / 2.0f,
                          (m_pos2.z + m_pos1.z) / 2.0f);
}


/**
 */
void CBBox::reset(void)
{
  m_pos1.x = 0.0f;
  m_pos1.y = 0.0f;
  m_pos1.z = 0.0f;

  m_pos2.x = 0.0f;
  m_pos2.y = 0.0f;
  m_pos2.z = 0.0f;

  return;
}


/**
 */
void CBBox::adjust(const CBBox & bbox)
{
  /* check on left bottom front corner */
  if (m_pos1.x > bbox.m_pos1.x)
  {
    m_pos1.x = bbox.m_pos1.x;
  }

  if (m_pos1.y > bbox.m_pos1.y)
  {
    m_pos1.y = bbox.m_pos1.y;
  }

  if (m_pos1.z > bbox.m_pos1.z)
  {
    m_pos1.z = bbox.m_pos1.z;
  }

  /* check on right top back corner */
  if (m_pos2.x < bbox.m_pos2.x)
  {
    m_pos2.x = bbox.m_pos2.x;
  }

  if (m_pos2.y < bbox.m_pos2.y)
  {
    m_pos2.y = bbox.m_pos2.y;
  }

  if (m_pos2.z < bbox.m_pos2.z)
  {
    m_pos2.z = bbox.m_pos2.z;
  }

  return;
}


/**
 */
void CBBox::adjust(const Maths::SVector3D & pt)
{
  /* minimum point */
  if (m_pos1.x > pt.x)
  {
    m_pos1.x = pt.x;
  }

  if (m_pos1.y > pt.y)
  {
    m_pos1.y = pt.y;
  }

  if (m_pos1.z > pt.z)
  {
    m_pos1.z = pt.z;
  }

  /* maximum point */
  if (m_pos2.x < pt.x)
  {
    m_pos2.x = pt.x;
  }

  if (m_pos2.y < pt.y)
  {
    m_pos2.y = pt.y;
  }

  if (m_pos2.z < pt.z)
  {
    m_pos2.z = pt.z;
  }

  return;
}


static Maths::SVector3D mul(double *mw, Maths::SVector3D vec)
{
  Maths::SVector3D res;

  res.x = mw[0] * vec.x + mw[4] * vec.y + mw[8] * vec.z;
  res.y = mw[1] * vec.x + mw[5] * vec.y + mw[9] * vec.z;
  res.z = mw[2] * vec.x + mw[6] * vec.y + mw[10] * vec.z;

  return res;
}


/**
 */
/*
 * This method was copied out from 0.a.d opensource game.
 * Though it is not currently used by us, you can find the
 * original code here: http://www.wildfiregames.com/0ad/
 */
bool CBBox::intersects(const Maths::SVector3D & origin,
                       const Maths::SVector3D & dir,
                       float *tmin,
                       float *tmax) const
{
#if 0
  float tmin = FLT_MAX;
  float tmax = FLT_MIN;

  qDebug() << "origin: " << origin << ", dir: " << dir;

  /*
   * m_pos1 -> left bottom back corner, has all coords smaller than m_pos2
   * m_pos2 -> right top front corner, has all coords greater than m_pos1
   */

  /* check the x-slab (the space between two parallel planes
     with x-coordinate equalt to 0) */
  if (dir.x == 0.0f)
  { // if the ray direction is parall with x-planes,
    // then its origin must lie within the x-slab
    if ((origin.x < m_pos1.x) || (origin.x > m_pos2.x))
    {
      return false;
    }
  }
  else
  {
    float t1 = (origin.x )
  }

  return true;
#endif

#if 1
  float t1,t2;
  float tnear,tfar;

  qDebug() << "pos1:   " << m_pos1 << ", pos2: " << m_pos2;

  if (dir.x == 0)
  {
    if (origin.x < m_pos1.x || origin.x > m_pos2.x)
    {
      return false;
    }
    else
    {
      tnear = (float) -FLT_MAX;
      tfar = (float) FLT_MAX;
    }
  }
  else
  {
    t1 = (m_pos1.x - origin.x) / dir.x;
    t2 = (m_pos2.x - origin.x) / dir.x;

    if (dir.x < 0)
    {
      tnear = t2;
      tfar = t1;
    }
    else
    {
      tnear = t1;
      tfar = t2;
    }

    if (tfar < 0)
    {
      return false;
    }
  }

  if (dir.y == 0 && (origin.y < m_pos1.y || origin.y > m_pos2.y))
  {
    return false;
  }
  else
  {
    t1 = (m_pos1.y - origin.y) / dir.y;
    t2 = (m_pos2.y - origin.y) / dir.y;

    if (dir.y < 0)
    {
      if (t2 > tnear) tnear = t2;
      if (t1 < tfar)  tfar = t1;
    }
    else
    {
      if (t1 > tnear)	tnear = t1;
      if (t2 < tfar)	tfar = t2;
    }

    if (tnear > tfar || tfar < 0)
    {
      return false;
    }
  }

  if (dir.z == 0 && (origin.z < m_pos1.z || origin.z > m_pos2.z))
  {
    return false;
  }
  else
  {
    t1 = (m_pos1.z - origin.z) / dir.z;
    t2 = (m_pos2.z - origin.z) / dir.z;

    if (dir.z < 0)
    {
      if (t2 > tnear) tnear = t2;
      if (t1 < tfar)  tfar = t1;
    }
    else
    {
      if (t1 > tnear) tnear = t1;
      if (t2 < tfar)  tfar = t2;
    }

    if (tnear > tfar || tfar < 0)
    {
      return false;
    }
  }

  *tmin = tnear;
  *tmax = tfar;

  return true;
#endif

#if 0
  float t1,t2;
  float tnear,tfar;

  qDebug() << "pos1:   " << m_pos1 << ", pos2: " << m_pos2;

  GLdouble mw[16];        // modelview matrix
  glGetDoublev(GL_MODELVIEW_MATRIX, mw);

  SVector3D m_min = mul(mw, m_pos1);
  SVector3D m_max = mul(mw, m_pos2);

  qDebug() << "pos1:   " << m_min << ", pos2: " << m_max;

  if (dir.x == 0)
  {
    if (origin.x < m_min.x || origin.x > m_max.x)
    {
      return false;
    }
    else
    {
      tnear = (float) -FLT_MAX;
      tfar = (float) FLT_MAX;
    }
  }
  else
  {
    t1 = (m_min.x - origin.x) / dir.x;
    t2 = (m_max.x - origin.x) / dir.x;

    if (dir.x < 0)
    {
      tnear = t2;
      tfar = t1;
    }
    else
    {
      tnear = t1;
      tfar = t2;
    }

    if (tfar < 0)
    {
      return false;
    }
  }

  if (dir.y == 0 && (origin.y < m_min.y || origin.y > m_max.y))
  {
    return false;
  }
  else
  {
    t1 = (m_min.y - origin.y) / dir.y;
    t2 = (m_max.y - origin.y) / dir.y;

    if (dir.y < 0)
    {
      if (t2 > tnear) tnear = t2;
      if (t1 < tfar)  tfar = t1;
    }
    else
    {
      if (t1 > tnear)	tnear = t1;
      if (t2 < tfar)	tfar = t2;
    }

    if (tnear > tfar || tfar < 0)
    {
      return false;
    }
  }

  if (dir.z == 0 && (origin.z < m_min.z || origin.z > m_max.z))
  {
    return false;
  }
  else
  {
    t1 = (m_min.z - origin.z) / dir.z;
    t2 = (m_max.z - origin.z) / dir.z;

    if (dir.z < 0)
    {
      if (t2 > tnear) tnear = t2;
      if (t1 < tfar)  tfar = t1;
    }
    else
    {
      if (t1 > tnear) tnear = t1;
      if (t2 < tfar)  tfar = t2;
    }

    if (tnear > tfar || tfar < 0)
    {
      return false;
    }
  }

  //tmin = tnear;
  //tmax = tfar;
#endif

  return true;
}


/**
 */
QDebug & operator<<(QDebug & debug, const CBBox & bbox)
{
  debug.nospace() << "CBBox(" << bbox.m_pos1 << ", " << bbox.m_pos2 << ")";
  return debug.maybeSpace();
}
