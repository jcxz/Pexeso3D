/*
 * Copyright (C) 2012-2013 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
 * Trackball implementation
 */

/*
 * The code in this file was taken from qt boxes demo
 * and adapted for our needs. For the original code see
 * http://doc.qt.digia.com/qt/demos-boxes.html
 */

#include <cmath>

#include <QLineF>

#include "CTrackBall.h"


const double PI = 3.14;



/**
 */
void CTrackBall::reset(void)
{
  m_rotation = QQuaternion();
  m_axis = QVector3D(0, 1, 0);
  m_angle = 0.0f;
  m_last_pos = QPointF(0.0f, 0.0f);

  return;
}


/**
 */
void CTrackBall::move(const QPointF & p, const QQuaternion & transformation)
{
  QVector3D last_pos_3D = QVector3D(m_last_pos.x(), m_last_pos.y(), 0.0f);
  float sqr_z = 1 - QVector3D::dotProduct(last_pos_3D, last_pos_3D);
  if (sqr_z > 0)
  {
    last_pos_3D.setZ(sqrt(sqr_z));
  }
  else
  {
    last_pos_3D.normalize();
  }

  QVector3D current_pos_3D = QVector3D(p.x(), p.y(), 0.0f);
  sqr_z = 1 - QVector3D::dotProduct(current_pos_3D, current_pos_3D);
  if (sqr_z > 0)
  {
    current_pos_3D.setZ(sqrt(sqr_z));
  }
  else
  {
    current_pos_3D.normalize();
  }

  m_axis = QVector3D::crossProduct(last_pos_3D, current_pos_3D);
  m_angle = 180 / PI * asin(sqrt(QVector3D::dotProduct(m_axis, m_axis)));
  m_axis.normalize();
  m_axis = transformation.rotatedVector(m_axis);
  m_rotation = QQuaternion::fromAxisAndAngle(m_axis, m_angle) * m_rotation;

  m_last_pos = p;

  return;
}
