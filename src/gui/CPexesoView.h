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
 * A widget to display the contents of the game scene
 */

#ifndef CPEXESOVIEW_H
#define CPEXESOVIEW_H

#include <QGraphicsView>


class QGLWidget;
class CPexesoScene;

/**
 * This class defines a view to display the pexeso
 * scene. It is necessary to handle certain events
 * differently than a regular QGraphicsView.
 */
class CPexesoView : public QGraphicsView
{
  Q_OBJECT

  public:
    /**
     * Constructor
     *
     * @param glwidget an OpenGL widget to get the OpenGL context
     * @param scene a PexesoScene to handle the game logic
     * @param parent a pointer to parent
     */
    CPexesoView(QGLWidget *glwidget, CPexesoScene *scene, QWidget *parent = 0);

  protected:
    /**
     * Reimplements the resize event so that the pexeso scene is resized
     * appropriately as well
     */
    virtual void resizeEvent(QResizeEvent *event);

    /**
     * Reimplements the show event to let the pexeso scene set up layout
     * of contained widgets
     */
    virtual void showEvent(QShowEvent *event);
};

#endif
