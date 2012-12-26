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
 * A widget that manages all menu screens
 */

#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QStackedWidget>


class QGLWidget;
class QStateMachine;
class CMainMenuWidget;
class CSinglePlayerMenuWidget;
class CMultiPlayerMenuWidget;
class CPexesoView;
class CPexesoScene;

/**
 * This class represents a main window,
 * that will allow to switch between widgets
 */
class CMainWindow : public QStackedWidget
{
  Q_OBJECT
  
  public:
    /**
     * Constructor
     */
    explicit CMainWindow(QGLWidget *glwidget, QWidget *parent = 0);

  private slots:
    void handleGameLoadTrans(void);

  private:
    CMainMenuWidget *m_main_menu;              /// main menu
    CSinglePlayerMenuWidget *m_single_player;  /// single player menu
    CMultiPlayerMenuWidget *m_multi_player;    /// multiplayer menu
    QGLWidget *m_glwidget;                     /// a widget to allow OpenGL rendering
    CPexesoView *m_pexeso_view;                /// a widget that displays the game scene
    CPexesoScene *m_pexeso_scene;              /// the game scene
    QStateMachine *m_machine;                  /// a machine to transition between menu screens
};

#endif
