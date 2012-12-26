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
 * Main Window implementation, implements also the screen transition logic
 */

#include <QStateMachine>
#include <QFinalState>
#include <QSignalTransition>
#include <QDir>
#include <QGLWidget>

#include "CMainWindow.h"
#include "CMainMenuWidget.h"
#include "CSinglePlayerMenuWidget.h"
#include "CMultiPlayerMenuWidget.h"
#include "CPexesoView.h"
#include "CPexesoScene.h"



/**
 */
CMainWindow::CMainWindow(QGLWidget *glwidget, QWidget *parent)
 : QStackedWidget(parent),
   m_main_menu(0),
   m_single_player(0),
   m_multi_player(0),
   m_glwidget(glwidget),
   m_pexeso_view(0),
   m_pexeso_scene(0),
   m_machine(0)
{
  PEXESO_ASSERT(m_glwidget != 0);

  /* create widgets */
  m_main_menu = new CMainMenuWidget;
  m_single_player = new CSinglePlayerMenuWidget;
  m_multi_player = new CMultiPlayerMenuWidget;
  /*
     according to dumentation it is possible to associate a single scene with multiple views,
     thus QGraphicsView will not an ownership over scene and we must do it manually.
     Also this SO post confirms this:
     http://stackoverflow.com/questions/4955743/does-a-qgraphicsview-take-ownership-over-its-associated-graphics-scene
  */
  m_pexeso_scene = new CPexesoScene(this);
  m_pexeso_view = new CPexesoView(m_glwidget, m_pexeso_scene);
  //m_pexeso->makeCurrent(); // this sadly does not work from constructor

  addWidget(m_main_menu);
  addWidget(m_single_player);
  addWidget(m_multi_player);
  addWidget(m_pexeso_view);

  /* create the state machine and states */
  m_machine = new QStateMachine(this);
  connect(m_machine, SIGNAL(finished()), SLOT(close()));

  QState *s_main_menu = new QState;
  QState *s_sp_menu = new QState;
  QState *s_mp_menu = new QState;
  QState *s_load = new QState;        // the game is loading
  QState *s_game = new QState;        // the game has begun
  QFinalState *sf = new QFinalState;  // final state

  /* assign actions to states */
  s_main_menu->assignProperty(this, "currentIndex", 0);  // main menu screen has index 0
  //s_main_menu->assignProperty(this, "windowTitle", "s_main_menu");

  s_sp_menu->assignProperty(this, "currentIndex", 1);    // single player screen has index 1
  //s_sp_menu->assignProperty(this, "windowTitle", "s_sp_menu");
  connect(s_sp_menu, SIGNAL(exited()), m_single_player, SLOT(clearError()));  // clear any errors when this state is exited

  s_mp_menu->assignProperty(this, "currentIndex", 2);    // multi player screen has index 2
  //s_mp_menu->assignProperty(this, "windowTitle", "s_mp_menu");

  connect(s_load, SIGNAL(entered()), SLOT(handleGameLoadTrans()));
  //s_load->assignProperty(this, "windowTitle", "s_load");

  s_game->assignProperty(this, "currentIndex", 3);       // game widget screen
  //s_game->assignProperty(this, "windowTitle", "s_game");

  /* transitions from main menu */
  s_main_menu->addTransition(m_main_menu, SIGNAL(exitTriggered()), sf);
  s_main_menu->addTransition(m_main_menu, SIGNAL(singlePlayerTriggered()), s_sp_menu);
  s_main_menu->addTransition(m_main_menu, SIGNAL(multiPlayerTriggered()), s_mp_menu);

  /* transitions from single player menu */
  s_sp_menu->addTransition(m_single_player, SIGNAL(backTriggered()), s_main_menu);
  s_sp_menu->addTransition(m_single_player, SIGNAL(startGameTriggered()), s_load);

  /* transitions from multiplayer menu */
  /* these transitions work like this: when in state s_mp_menu and multiplayer
     menu screen triggeres a signal backTriggered() then the state machine will
     transition to s_main_menu state */
  s_mp_menu->addTransition(m_multi_player, SIGNAL(backTriggered()), s_main_menu);

  /* load transitions */
  s_load->addTransition(m_pexeso_scene, SIGNAL(errorTriggered()), s_sp_menu);
  s_load->addTransition(m_pexeso_scene, SIGNAL(startTriggered()), s_game);

  /* transitons from in-game menu */
  s_game->addTransition(m_pexeso_scene, SIGNAL(mainMenuTriggered()), s_main_menu);
  s_game->addTransition(m_pexeso_scene, SIGNAL(quitTriggered()), sf);
  s_game->addTransition(m_pexeso_scene, SIGNAL(gameOverTriggered()), s_main_menu);

  /* add states to state machine and start it */
  m_machine->addState(s_main_menu);
  m_machine->addState(s_sp_menu);
  m_machine->addState(s_mp_menu);
  m_machine->addState(s_load);
  m_machine->addState(s_game);
  m_machine->addState(sf);

  m_machine->setInitialState(s_main_menu);

  m_machine->start();
}


/**
 * This slot will handle starting the game
 */
void CMainWindow::handleGameLoadTrans(void)
{
  PEXESO_ASSERT(m_glwidget != 0);

  /*
   * Because of newGame method uses OpenGL internally we need to make
   * the OpenGL context of the glwidget current before calling it.
   */
  m_glwidget->makeCurrent();

  if (!m_pexeso_scene->newGame(m_single_player->getSelectedScene()))
  {
    qDebug() << "error loading game from from scene file:"
             << m_single_player->getSelectedScene();
    m_single_player->setError(tr("Failed to start the game \"%1\": %2")
                              .arg(m_single_player->getSelectedScene())
                              .arg(m_pexeso_scene->getErrorString()));
  }
  else
  {
    m_single_player->clearError();
  }

  return;
}
