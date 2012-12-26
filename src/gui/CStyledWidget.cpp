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
 * Implements the base stylable widget
 */

#include <QStyleOption>
#include <QPainter>

#include "Global.h"
#include "CStyledWidget.h"


/**
 */
void CStyledWidget::paintEvent(QPaintEvent *event)
{
  PEXESO_UNUSED(event);

  /*
   * this code was inspired by qt's docuentation.
   * see: http://developer.qt.nokia.com/doc/qt-4.8/stylesheet-reference.html
   */

  QPainter p(this);
  QStyleOption opt;

  opt.init(this);
  style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

  return;
}
