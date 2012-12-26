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
 * Game Menu Widget itnerface
 */

#ifndef CGAMEMENUWIDGET_H
#define CGAMEMENUWIDGET_H

#include "CStyledWidget.h"


class QPushButton;
class QLabel;

/**
 * This class represents an in-game menu
 */
class CGameMenuWidget : public CStyledWidget
{
  Q_OBJECT

  public:
    /**
     * Constructor
     */
    explicit CGameMenuWidget(QWidget *parent = 0);

  signals:
    void quitTriggered(void);
    void mainMenuTriggered(void);
    void resumeTriggered(void);

  private:
    QLabel *m_banner;             /// a menu banner (picture, whatever)
    QPushButton *m_pb_resume;     /// a button to resume the game
    QPushButton *m_pb_main_menu;  /// a button to return to main menu
    QPushButton *m_pb_quit;       /// a button to quit the game
};

#endif
