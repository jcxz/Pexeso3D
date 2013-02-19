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
 * Implementation of the main game class
 */

#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "CPexesoScene.h"
#include "CGameMenuWidget.h"
#include "CGameOverWidget.h"
#include "CTurnManager.h"
#include "CBaseAiAgent.h"


/* global variables private to this module */
namespace {
  /* defines how many miliseconds should each frame take */
  const int g_frame_interval = 33;
}




/**
 */
CPexesoScene::CPexesoScene(QObject *parent)
  : QGraphicsScene(parent),
    m_state(STATE_STOPPED),
    m_error(ERR_OK),
    m_show_stencil(false),
    m_transl_z(0),
    m_framecount(0),
    m_timer_id(0),
    m_last_sel_mod(0),
    m_scene(),
    m_renderer(),
    m_track_ball(),
    m_tm(0),
    m_ai(0),
    m_pb_menu(0),
    m_menu(0),
    m_game_over(0),
    m_p1_score(0),
    m_p2_score(0)
{
  /* create widgets */
  QPushButton *pb_menu = new QPushButton(tr("Menu"));
  pb_menu->setObjectName("ingame_menu_pb");
  connect(pb_menu, SIGNAL(clicked()), SLOT(handleMenuBtnClicked()));

  CGameMenuWidget *menu = new CGameMenuWidget;
  menu->setVisible(false);
  connect(menu, SIGNAL(resumeTriggered()),   SLOT(handleResumeBtnClicked()));
  connect(menu, SIGNAL(mainMenuTriggered()), SIGNAL(mainMenuTriggered()));
  connect(menu, SIGNAL(mainMenuTriggered()), SLOT(stopGame()));
  connect(menu, SIGNAL(quitTriggered()),     SIGNAL(quitTriggered()));
  connect(menu, SIGNAL(quitTriggered()),     SLOT(stopGame()));

  CGameOverWidget *game_over = new CGameOverWidget;
  game_over->setVisible(false);
  connect(game_over, SIGNAL(continueTriggered()), SIGNAL(gameOverTriggered()));

  QLabel *p1_score = new QLabel(tr("Player\n0"));
  p1_score->setAlignment(Qt::AlignCenter);
  p1_score->setObjectName("ingame_p1_score_lbl");

  QLabel *p2_score = new QLabel(tr("CPU\n0"));
  p2_score->setAlignment(Qt::AlignCenter);
  p2_score->setObjectName("ingame_p2_score_lbl");

  /* add gui widgets to the scene */
  m_pb_menu = addWidget(pb_menu);
  m_menu = addWidget(menu);
  m_game_over = addWidget(game_over);
  m_p1_score = addWidget(p1_score);
  m_p2_score = addWidget(p2_score);

  /* intialize game classes */
  m_ai = new CBaseAiAgent(m_scene.getModelVectorPointer());
  m_tm = new CTurnManager(m_ai);

  m_renderer.setTrackBall(&m_track_ball);

  /* trap the signal when a scene rect is changed */
  connect(this, SIGNAL(sceneRectChanged(QRectF)), SLOT(handleSceneRectChange(QRectF)));
}


/**
 */
CPexesoScene::~CPexesoScene(void)
{
  delete m_ai;
  delete m_tm;
}


/**
 */
bool CPexesoScene::newGame(const QString & scene_file, EDifficulty difficulty)
{
  qDebug() << PEXESO_FUNC;

  m_error = ERR_OK;  // reset the error flag

  /* check that the game is stopped before starting a new game */
  if (m_state == STATE_RUNNING)
  {
    qDebug() << "Stop the game before continuing";
    m_error = ERR_RUNNING;
    emit errorTriggered();
    return false;
  }

  /* cleanup (just in case) */
  //delete m_ai; m_ai = 0;

  /* load the scene */
  if (!m_scene.loadFromXML(scene_file))
  {
    qDebug() << m_scene << scene_file;
    m_error = ERR_SCENE;
    emit errorTriggered();
    return false;
  }
   
  /* reinitialize the game */
  m_show_stencil = false;
  m_last_sel_mod = 0;
  m_framecount = 0;
  m_track_ball.reset();
  m_renderer.setMode(CRenderer::RM_NORMAL);
  m_tm->init();
  m_ai->initModels();
  m_ai->setSkill((int) difficulty);

  /* start the frame counter */
  m_timer_id = startTimer(g_frame_interval);
  if (!m_timer_id)
  {
    qDebug() << "Failed to start timer";
    m_error = ERR_TIMER;
    emit errorTriggered();
    return false;
  }

  m_state = STATE_RUNNING;  // make the game started

  /* the game was successfully started */
  emit startTriggered();

  return true;
}


/**
 */
void CPexesoScene::stopGame(void)
{
  qDebug() << PEXESO_FUNC;
  killTimer(m_timer_id);   // kill the main loop
  m_state = STATE_STOPPED;
  //emit stopTriggered();  // maybe emit a signal
  return;
}


/**
 */
void CPexesoScene::handleMenuBtnClicked(void)
{
  if (m_state == STATE_RUNNING)
  {
    m_state = STATE_PAUSED;
  }

  return;
}


/**
 */
void CPexesoScene::handleResumeBtnClicked(void)
{
  if (m_state == STATE_PAUSED)
  {
    m_state = STATE_RUNNING;
  }

  return;
}


/**
 */
void CPexesoScene::handleSceneRectChange(const QRectF & rect)
{
  qreal w = rect.width();
  qreal h = rect.height();
  qreal margin = 5.0f;

  /* main menu button position */
  m_pb_menu->setPos((w - m_pb_menu->geometry().width()) / 2.0f, margin);

  /* score labels positions */
  m_p1_score->setPos(margin, h - m_p1_score->geometry().height() - margin);
  m_p2_score->setPos(w - m_p2_score->geometry().width() - margin,
                     h - m_p2_score->geometry().height() - margin);

  /* game menu position */
  m_menu->setPos((w - m_menu->geometry().width()) / 2.0f,
                 (h - m_menu->geometry().height()) / 2.0f);

  /* game over/victory screen position */
  m_game_over->setPos((w - m_game_over->geometry().width()) / 2.0f,
                      (h - m_game_over->geometry().height()) / 2.0f);

  /* resize the viewport */
  //m_renderer.resize(w, h);

  update();

  return;
}


/**
 */
void CPexesoScene::findOptimalDistanceFromCamera(int w, int h)
{
  /* TODO: fix this method */

  /* the code in this method is supposed to
     place the scene in optimal distance from the camera
     before it is shown the first time.
     The reason why this is not done in newGame method
     is that Qt will not return correct widget geometry
     until the widget is shown */

  qDebug() << PEXESO_FUNC;

  if (m_error != ERR_OK)
  {
    qDebug() << PEXESO_FUNC
             << "Not continuing because of the following error: "
             << errorToString(m_error);
    return;
  }

  /* find out an optimal initial distance for the scene */
  const CBBox & scene_bbox = m_scene.getBBox();
  float scene_height = scene_bbox.getHeight();
  float scene_width = scene_bbox.getWidth();

  qDebug() << "scene_height: " << scene_height
           << "scene_width: " << scene_width
           << ", height(): " << height()
           << ", width(): " << height()
           << ", height() / scene_height" << (height() / scene_height);

  const Maths::SVector3D scene_c = scene_bbox.getCenter();

  if ((scene_width == 0) || (scene_height == 0))
  {
    m_transl_z = 0;
  }
  else if (scene_width > scene_height)
  {
    m_transl_z =  (w / (scene_width)) * -(fabs(scene_bbox.getMax().x) + fabs(scene_bbox.getMin().x));
  }
  else
  {
    m_transl_z =  (h / (scene_height)) * -(fabs(scene_bbox.getMax().y) + fabs(scene_bbox.getMin().y));
  }

  qDebug() << "transl_z: "
          << m_transl_z
          << ","
          << (height() / scene_height)
          << ","
          << (fabs(scene_bbox.getMax().y) + fabs(scene_bbox.getMin().y))
          << ","
          << 2 * scene_c.z
          << ", geometry: "
          << w
          << "x"
          << h;

  return;
}


/**
 */
void CPexesoScene::keyPressEvent(QKeyEvent *event)
{
  /* check on proper initialization */
  if (m_error != ERR_OK)
  {
    event->ignore();
    qDebug() << PEXESO_FUNC
             << "Not continuing because of the following error: "
             << errorToString(m_error);
    return;
  }

  /* check on the game state */
  if (m_state != STATE_RUNNING)
  {
    event->ignore();
    qDebug() << PEXESO_FUNC
             << "game"
             << ((m_state == STATE_STOPPED) ? "stopped" : "paused");
    return;
  }

  /* handle the specific key press event */
  switch (event->key())
  {
    case Qt::Key_Space:
      m_track_ball.reset();
      break;

    case Qt::Key_Plus:
      m_renderer.toggleNextMode();
      break;

    case Qt::Key_Minus:
      m_renderer.togglePrevMode();
      break;

    case Qt::Key_S:
      m_show_stencil ^= true;  // toggle stencil buffer display
      break;

    default:
      /* all other events are left to qt */
      QGraphicsScene::keyPressEvent(event);
      return;
  }

  event->accept();  // accept the event if it was our event

  return;
}


/**
 */
void CPexesoScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  /* let first the qgraphicsscene handle the event */
  QGraphicsScene::mousePressEvent(event);
  if (event->isAccepted())
  {
    return;
  }

  /* check on proper initialization */
  if (m_error != ERR_OK)
  {
    event->ignore();
    qDebug() << PEXESO_FUNC
             << "Not continuing because of the following error: "
             << errorToString(m_error);
    return;
  }

  /* check on the game state */
  if (m_state != STATE_RUNNING)
  {
    event->ignore();
    qDebug() << PEXESO_FUNC
             << "game"
             << ((m_state == STATE_STOPPED) ? "stopped" : "paused");
    return;
  }

  /* process the event */
  if (event->button() == Qt::LeftButton)
  {
    m_track_ball.push(pixelPosToViewPos(event->scenePos()));
    event->accept();
  }
  else if (event->button() == Qt::RightButton)
  {
    /* deselect the last selection */
    if (m_last_sel_mod)
    {
      m_last_sel_mod->setSelected(false);
    }

    /*if it's P1's turn, highlight selected model*/
    if (m_tm->isP1Turn())
    {
      CBaseModel *model = m_scene.calcIntersection(event->scenePos().x(), event->scenePos().y());
      if (model != NULL)
      {
        qDebug() << "model == " << *model;
        model->setSelected();
        m_tm->setModel(model);   // send the selected model to TurnManager
        m_last_sel_mod = model;
      }
      else
      {
        qDebug() << "model == NULL";
        m_last_sel_mod = NULL;
      }
    }

    event->accept();
  }

  return;
}


/**
 */
void CPexesoScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
  /* let first the QGraphicsScene handle the event */
  QGraphicsScene::mouseMoveEvent(event);
  if (event->isAccepted())
  {
    return;
  }

  /* check on proper initialization */
  if (m_error != ERR_OK)
  {
    event->ignore();
    qDebug() << PEXESO_FUNC
             << "Not continuing because of the following error: "
             << errorToString(m_error);
    return;
  }

  /* check on the game state */
  if (m_state != STATE_RUNNING)
  {
    event->ignore();
    qDebug() << PEXESO_FUNC
             << "game"
             << ((m_state == STATE_STOPPED) ? "stopped" : "paused");
    return;
  }

  /* process the event */
  if (event->buttons() & Qt::LeftButton)
  {
    m_track_ball.move(pixelPosToViewPos(event->scenePos()), QQuaternion());
    event->accept();
  }

  return;
}


/**
 */
void CPexesoScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
  /* let QGraphicsScene handle the event as first */
  QGraphicsScene::wheelEvent(event);
  if (event->isAccepted())
  {
    return;
  }

  /* check on proper initialization */
  if (m_error != ERR_OK)
  {
    event->ignore();
    qDebug() << PEXESO_FUNC
             << "Not continuing because of the following error: "
             << errorToString(m_error);
    return;
  }

  /* check on the game state */
  if (m_state != STATE_RUNNING)
  {
    event->ignore();
    qDebug() << PEXESO_FUNC
             << "game"
             << ((m_state == STATE_STOPPED) ? "stopped" : "paused");
    return;
  }

  /* handle the event */
  m_transl_z += event->delta();
  qDebug() << "delta: " << event->delta() << " m_transl_z == " << m_transl_z;

  event->accept();
  
  return;
}


/**
 */
void CPexesoScene::drawBackground(QPainter *painter, const QRectF & rect)
{
  painter->beginNativePainting();

  /* intialize and setup renderer */
  if (!m_renderer.init(rect.width(), rect.height()))
  {
    qDebug() << "Failed to initialize renderer";
    m_error = ERR_RENDERER;
    return;
  }

  /* render game */
  m_renderer.renderBackground(m_scene);

#ifdef PEXESO_PROJ_ORTHO
  glTranslatef((width() / 2.0f), (height() / 2.0f), 0.0f);
  glScalef(0.01 * m_transl_z, 0.01 * m_transl_z, 0.01 * m_transl_z);
#else
  glTranslatef(0.0f, 0.0f, m_transl_z);
#endif

  QLabel *p1_score = static_cast<QLabel *>(m_p1_score->widget());
  QLabel *p2_score = static_cast<QLabel *>(m_p2_score->widget());
  CRenderer::ERenderMode old_mode = m_renderer.getMode();

  /* if the computer AI is on turn render the scene as a series of wireframes */
  if (m_tm->isP1Active())
  {
    p1_score->setEnabled(true);
    p1_score->setText(tr("Player\n%1").arg(m_tm->getP1Score()));
    p2_score->setEnabled(false);
  }
  else
  {
    m_renderer.setMode(CRenderer::RM_WIRED);
    p1_score->setEnabled(false);
    p2_score->setEnabled(true);
    p2_score->setText(tr("CPU\n%1").arg(m_tm->getP2Score()));
  }

  m_renderer.renderScene(m_scene);
  //m_renderer.renderXYZAxes();

  m_renderer.setMode(old_mode);  // and restore the mode

  /* render HUD and menus */
  if (m_state == STATE_PAUSED)
  {
    m_renderer.dimScreen();
    m_menu->setVisible(true);
  }
  else if (m_state == STATE_STOPPED)
  {
    if (m_tm->getP1Score() > m_tm->getP2Score())
    {
      CGameOverWidget *game_over = static_cast<CGameOverWidget *>(m_game_over->widget());
      game_over->setType(CGameOverWidget::TYPE_VICTORY);
    }
    else
    {
      CGameOverWidget *game_over = static_cast<CGameOverWidget *>(m_game_over->widget());
      game_over->setType(CGameOverWidget::TYPE_DEFEAT);
    }
    m_renderer.dimScreen();
    m_game_over->setVisible(true);
  }
  else
  {
    CGameMenuWidget *menu = static_cast<CGameMenuWidget *>(m_menu->widget());
    CGameOverWidget *game_over = static_cast<CGameOverWidget *>(m_game_over->widget());
    menu->setVisible(false);
    game_over->setVisible(false);
  }

  if (m_show_stencil)
  {
    m_renderer.renderStencilBuffer();
  }

  /* deintialize the renderer */
  if (!m_renderer.deinit())
  {
    qDebug() << "Warning: Failed to deintialize renderer";
  }

  painter->endNativePainting();

  return;
}


/**
 */
void CPexesoScene::timerEvent(QTimerEvent *event)
{
  /* check on proper initialization */
  if (m_error != ERR_OK)
  {
    event->ignore();
    qDebug() << PEXESO_FUNC
             << "Not continuing because of the following error: "
             << errorToString(m_error);
    return;
  }

  /* check on the game state */
  if (m_state != STATE_RUNNING)
  {
    event->ignore();
    qDebug() << PEXESO_FUNC
             << "game"
             << ((m_state == STATE_STOPPED) ? "stopped" : "paused");
    return;
  }

  /* send the time event to turnmanager */
  if (m_tm->event(m_framecount))
  {
    qDebug() << "Game has ended.";
    stopGame();
  }

  /* draw the next frame */
  update();

  m_framecount++;

  return;
}


/**
 */
const char *CPexesoScene::errorToString(EError error)
{
  static const char *err_strings[] = {
    "No error",                                     // ERR_OK
    "Failed to start timer",                        // ERR_TIMER
    "Failed to load the scene",                     // ERR_SCENE
    "Failed to initialize renderer",                // ERR_RENDERER
    "Attempted to start a new game while playing",  // ERR_RUNNING
    "Unknown error"                                 // ERR_UNKNOWN
  };

  if ((error < ERR_OK) || (error > ERR_UNKNOWN))
  {
    return err_strings[ERR_UNKNOWN];
  }

  return err_strings[error];
}
