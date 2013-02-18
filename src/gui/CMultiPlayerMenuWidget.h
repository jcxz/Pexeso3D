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
 * A widget to display multiplayer menu
 */

#ifndef CMULTIPLAYERMENUWIDGET_H
#define CMULTIPLAYERMENUWIDGET_H

#include "CStyledWidget.h"


class QPushButton;
class CImageWidget;
class QLabel;

/**
 */
class CMultiPlayerMenuWidget : public CStyledWidget
{
  Q_OBJECT

  public:
    explicit CMultiPlayerMenuWidget(QWidget *parent = 0);

  signals:
    void hostGameTriggered(void);
    void joinGameTriggered(void);
    void backTriggered(void);

  private:
    QPushButton *m_pb_join_game;  /// join an existing lan game
    QPushButton *m_pb_host_game;  /// create a new lan game
    QPushButton *m_pb_back;       /// exit button
    QLabel *m_header;         /// game logo
};

#endif
