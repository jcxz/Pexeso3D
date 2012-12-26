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
 * A widget to display credits information
 */

#ifndef CCREDITSLABEL_H
#define CCREDITSLABEL_H

#include <QLabel>

#include "Global.h"


/**
 * This class provides a panel with credits
 */
class CCreditsLabel : public QLabel
{
  Q_OBJECT

  public:
    explicit CCreditsLabel(const QString & text = tr(g_credits), QWidget *parent = 0)
      : QLabel(text, parent)
    {
      //setWordWrap(true);
    }
};

#endif
