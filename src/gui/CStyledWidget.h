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
 * A base widget for all widgets that can be styled with Qt stylesheets
 */

#ifndef CSTYLEDWIDGET_H
#define CSTYLEDWIDGET_H

#include <QWidget>


/**
 * This class subclasses QWidget to override
 * its paint method and provide full support for
 * qt stylesheets
 */
class CStyledWidget : public QWidget
{
  Q_OBJECT

  public:
    explicit CStyledWidget(QWidget *parent = 0)
      : QWidget(parent)
    {
    }

  protected:
    virtual void paintEvent(QPaintEvent *event);
};

#endif
