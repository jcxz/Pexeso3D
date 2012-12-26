/*
 * Copyright (C) 2012 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
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
 * Trackball interface
 */

/*
 * The code in this file was taken from qt boxes demo
 * and adapted for our needs. For the original code see
 * http://doc.qt.digia.com/qt/demos-boxes.html
 */

#ifndef CTRACKBALL_H
#define CTRACKBALL_H

#include <QVector3D>
#include <QQuaternion>


/**
 * This class takes care of scene rotations
 */
class CTrackBall
{
  public:
    /* The type of trackball */
    enum ETrackMode {
      Plane = 0,
      Sphere
    };

  public:
    /**
     * Constructor
     */
    CTrackBall(ETrackMode mode = Sphere)
      : m_mode(mode),
        m_angle(0.0f),
        m_axis(QVector3D(0, 1, 0)),
        m_last_pos(0.0f, 0.0f),
        m_rotation()
    {
    }

    /**
     * Constructor
     */
    CTrackBall(float angle, const QVector3D & axis, ETrackMode mode = Sphere)
      : m_mode(mode),
        m_angle(angle),
        m_axis(axis),
        m_last_pos(0.0f, 0.0f),
        m_rotation()
    {
    }

    /**
     * This method will return the currently set rotation
     */
    QQuaternion getRotation(void) const
    {
      return QQuaternion::fromAxisAndAngle(m_axis, m_angle) * m_rotation;
    }

    /**
     * This method will reset the trackball position
     */
    void reset(void);

    // coordinates have to be in [-1,1]x[-1,1] for easier math
    void push(const QPointF & p, const QQuaternion & transformation);
    void move(const QPointF & p, const QQuaternion & transformation);
    void release(const QPointF & p, const QQuaternion & transformation);

  private:
    ETrackMode m_mode;          /// trackball type
    float m_angle;
    QVector3D m_axis;
    QPointF m_last_pos;
    QQuaternion m_rotation;
};

#endif
