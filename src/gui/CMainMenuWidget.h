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
 * A widget to display main menu
 */

#ifndef CMAINMENUWIDGET_H
#define CMAINMENUWIDGET_H

#include "CStyledWidget.h"



class QPushButton;
class CCreditsLabel;
class CImageWidget;

/**
 */
class CMainMenuWidget : public CStyledWidget
{
  Q_OBJECT

  public:
    explicit CMainMenuWidget(QWidget *parent = 0);

  signals:
    void singlePlayerTriggered(void);
    void multiPlayerTriggered(void);
    void highScoresTriggered(void);
    void exitTriggered(void);

  private:
    QPushButton *m_pb_single_player;   /// single player button
    QPushButton *m_pb_multi_player;    /// multiplayer button
    QPushButton *m_pb_highscores;      /// high scores button
    QPushButton *m_pb_exit;            /// exit button
    CCreditsLabel *m_credits;          /// credids panel
    CImageWidget *m_logo;              /// game logo
};

#endif

