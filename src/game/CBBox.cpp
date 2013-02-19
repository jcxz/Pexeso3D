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
 * Implementation of an axis aligned bounding box
 */

#include "CBBox.h"



/**
 */
void CBBox::reset(void)
{
  m_min.x = 0.0f;
  m_min.y = 0.0f;
  m_min.z = 0.0f;

  m_max.x = 0.0f;
  m_max.y = 0.0f;
  m_max.z = 0.0f;

  return;
}


/**
 */
void CBBox::adjust(const CBBox & bbox)
{
  /* check on left bottom front corner */
  if (m_min.x > bbox.m_min.x)
  {
    m_min.x = bbox.m_min.x;
  }

  if (m_min.y > bbox.m_min.y)
  {
    m_min.y = bbox.m_min.y;
  }

  if (m_min.z > bbox.m_min.z)
  {
    m_min.z = bbox.m_min.z;
  }

  /* check on right top back corner */
  if (m_max.x < bbox.m_max.x)
  {
    m_max.x = bbox.m_max.x;
  }

  if (m_max.y < bbox.m_max.y)
  {
    m_max.y = bbox.m_max.y;
  }

  if (m_max.z < bbox.m_max.z)
  {
    m_max.z = bbox.m_max.z;
  }

  return;
}


/**
 */
void CBBox::adjust(const Maths::SVector3D & pt)
{
  /* minimum point */
  if (m_min.x > pt.x)
  {
    m_min.x = pt.x;
  }

  if (m_min.y > pt.y)
  {
    m_min.y = pt.y;
  }

  if (m_min.z > pt.z)
  {
    m_min.z = pt.z;
  }

  /* maximum point */
  if (m_max.x < pt.x)
  {
    m_max.x = pt.x;
  }

  if (m_max.y < pt.y)
  {
    m_max.y = pt.y;
  }

  if (m_max.z < pt.z)
  {
    m_max.z = pt.z;
  }

  return;
}


/**
 */
QDebug & operator<<(QDebug & debug, const CBBox & bbox)
{
  debug.nospace() << "CBBox(" << bbox.m_min << ", " << bbox.m_max << ")";
  return debug.maybeSpace();
}
