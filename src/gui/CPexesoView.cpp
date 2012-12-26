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
 * Implements a view widget to display the game scene
 */

#include <QGLWidget>

#include "Global.h"
#include "CPexesoView.h"
#include "CPexesoScene.h"



/**
 */
CPexesoView::CPexesoView(QGLWidget *glwidget, CPexesoScene *scene, QWidget *parent)
  : QGraphicsView(parent)
{
  PEXESO_ASSERT(glwidget != 0);
  PEXESO_ASSERT(scene != 0);

  setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
  //setRenderHints(QPainter::SmoothPixmapTransform);

  setViewport(glwidget);
  setViewportUpdateMode(QGraphicsView::FullViewportUpdate);  // allways redraw the whole view
  setScene(scene);
}


/**
 */
void CPexesoView::resizeEvent(QResizeEvent *event)
{
  PEXESO_ASSERT(scene() != 0);

  scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));

  QGraphicsView::resizeEvent(event);

  return;
}


/**
 */
void CPexesoView::showEvent(QShowEvent *event)
{
  CPexesoScene *pexeso_scene = static_cast<CPexesoScene *>(scene());

  PEXESO_ASSERT(pexeso_scene != 0);

  pexeso_scene->setSceneRect(QRect(0, 0, width(), height()));
  pexeso_scene->findOptimalDistanceFromCamera(width(), height());

  QGraphicsView::showEvent(event);

  return;
}
