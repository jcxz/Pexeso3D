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
 * Definition of the main game class
 */

#ifndef CPEXESOSCENE_H
#define CPEXESOSCENE_H

#include <QGraphicsScene>

#include "CScene.h"
#include "CRenderer.h"
#include "CTrackBall.h"



class CTurnManager;
class CBaseAiAgent;

/**
 * This class glues together all other game classes and
 * implements the game loop
 */
class CPexesoScene : public QGraphicsScene
{
  Q_OBJECT

  public:
    /** This enumeration defines possible error conditions */
    enum EError {
      ERR_OK = 0,         /// no error occured
      ERR_TIMER,          /// failed to start timer
      ERR_SCENE,          /// failed to load the scene
      ERR_RENDERER,       /// failed to initialize renderer
      ERR_RUNNING,        /// an attempt to start a new game while playing
      ERR_UNKNOWN         /// an unspecied error occured
    };

    /** This enumeration defines difficulty constants */
    enum EDifficulty {
      EASY = 0,           /// lowest difficulty level
      MEDIUM,             /// medium difficulty level
      HARD                /// highest difficulty level
    };

  private:
    /** This enumeration defines the current game state */
    enum EState {
      STATE_STOPPED = 0,  /// the game is stopped (no even started or somebody won)
      STATE_RUNNING,      /// the game is running
      STATE_PAUSED        /// the game is paused (for example amenu is displayed)
    };

  public:
    /**
     * This function call makeCurrent in constructor,
     * so you can youse OpenGL function since declaring it
     */
    explicit CPexesoScene(QObject *parent = 0);

    /**
     * Destructor
     */
    ~CPexesoScene(void);

    /**
     * This method will return a textual description of an error in game
     *
     * @retrun a string with error message
     */
    QString getErrorString(void) const
    {
      return errorToString(m_error);
    }

  public slots:
    /**
     * This slot will start a new game
     * On error errorTriggered signal is emitted.
     * On success startTriggered signal is emitted.
     *
     * @param scene_file the game to be started
     * @param difficulty difficulty of the game
     *
     * @return true of the game could be initialised,
     *         false otherwise
     */
    bool newGame(const QString & scene_file, EDifficulty difficulty = MEDIUM);

    /**
     * This slot will stop the game
     */
    void stopGame(void);

  signals:
    void mainMenuTriggered(void);
    void quitTriggered(void);
    void errorTriggered(void);
    void startTriggered(void);
    void gameOverTriggered(void);

  private slots:
    void handleMenuBtnClicked(void);
    void handleResumeBtnClicked(void);

    /**
     * A slot to layout the game widgets
     */
    void handleSceneRectChange(const QRectF & rect);

  protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void wheelEvent(QGraphicsSceneWheelEvent *event);

    virtual void drawBackground(QPainter *painter, const QRectF & rect);

    /**
     * This event handler takes care of redrawing the
     * screen and updating the game logic and ai
     */
    virtual void timerEvent(QTimerEvent *event);

  private:
    /**
     * This method will convert a pixel from screen to range
     * [-1,1]x[-1,1] required by trackball
     *
     * @param p a point on the screen
     *
     * @return a point in suitable for passing to CTrackBall
     */
    inline QPointF pixelPosToViewPos(const QPointF & p)
    {
      return QPointF(2.0 * float(p.x()) / width() - 1.0,
                    1.0 - 2.0 * float(p.y()) / height());
    }

    /**
     * This function will ty to guess a good intial distance
     * from camera where to place the scene
     *
     * @param w window width
     * @param h window height
     */
    void findOptimalDistanceFromCamera(int w, int h);

    /**
     * This method will convert an error constant to an error message
     */
    static const char *errorToString(EError error);

  private:
    /* game stuff */
    EState m_state;                     /// determines the current game state
    EError m_error;                     /// an internal error
    bool m_show_stencil;                /// whether to display stencil buffer contents
    GLfloat m_transl_z;                 /// determines translation in z-axis (zoom)
    unsigned long int m_framecount;     /// for counting frames
    int m_timer_id;                     /// the id of timer that drives the main loop
    CBaseModel *m_last_sel_mod;         /// for drawing seletion box
    CScene m_scene;                     /// game scene
    CRenderer m_renderer;               /// scene renderer
    CTrackBall m_track_ball;            /// to move and rotate the scene
    CTurnManager *m_tm;                 /// a class to manage game logic
    CBaseAiAgent *m_ai;                 /// human player's oponent type

    /* ui widgets */
    QGraphicsProxyWidget *m_pb_menu;    /// a button to trigger game's menu
    QGraphicsProxyWidget *m_menu;       /// an in-game menu
    QGraphicsProxyWidget *m_game_over;  /// a widget to display a game over screen
    QGraphicsProxyWidget *m_p1_score;   /// a widget to display first player's score
    QGraphicsProxyWidget *m_p2_score;   /// a widget to display second' player's score

    /*
     * This is used so that CPexesoView has an acces to findOptimalDistanceFromCamera()
     * function. This function has to be called before anything si shown on the screen
     * otherwise it creates some undesired visual artifacts.
     * It would probably not be wise to expose this internal function
     * to the whole world and these two classes are already quite tightly coupled anyways.
     */
    friend class CPexesoView;
};

#endif
