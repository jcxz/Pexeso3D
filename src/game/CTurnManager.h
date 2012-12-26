/*
 * Copyright (C) 2012 Josef Rudolf <xrudol04@stud.fit.vutbr.cz>
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
 * Defines turn manager interface
 */

#ifndef CTURNMANAGER_H
#define CTURNMANAGER_H

#include <QList>
#include <QDebug>

#include "CBaseAiAgent.h"


//class CBaseModel;

/**
 * This class represents a game turn manager
 */
class CTurnManager
{
  public:
    /** This enumeration defines current game state */
    enum GState {
      GS_P1_1 = 0,  /// waiting for P1's 1st choice
      GS_P1_2,      /// waiting for P1's 2st choice
      GS_P1_W0,     /// shedule delay to show P1's choices uncovered for a moment before evaluation
      GS_P1_W,      /// delay and evaluate P1's two choices
      GS_P2_1,      /// waiting for P2's 1st choice
      GS_P2_2,      /// waiting for P2's 2st choice
      GS_P2_W,      /// delay and evaluate P2's two choices
      GS_OVER       /// game has ended
    };

  public:
    /**
     * Constructor
     */
    explicit CTurnManager(CBaseAiAgent *p2);
    
    /**
     * Initialization method.
     */
    void init();

    /**
     * Method which is supposed to be called before rendering of each frame.
     * Handles AI's turn and delays.
     * Returns true if game has ended.
     */
    bool event(int framenumber);

    /**
     * Method which is supposed to be called when 1st player selected a model
     */
    void setModel(CBaseModel *model);

    /**
     * Return whether it is Player 1's turn
     */
    bool isP1Turn() const {
      return (m_current_state == GS_P1_1 || m_current_state == GS_P1_2);
    }
    
    /**
     * Return true if player 1 is active (used especially for score displaying)
     */
    bool isP1Active() const {
      return (m_current_state == GS_P1_1 || m_current_state == GS_P1_2 || m_current_state == GS_P1_W0 || m_current_state == GS_P1_W);
    }
    
    /**
     * Return the score of the first player
     */
    int getP1Score(void) const {
      return m_p1_score;
    }

    /**
     * Return second player's score
     */
    int getP2Score(void) const {
      return m_p2_score;
    }

  private:
    /**
     * Returns true if m1 and m2 have equal texture indeX;
     */
    bool endTurn(CBaseModel *m1, CBaseModel *m2);

  protected:
    bool m_game_over;              /// determines whether the game has ended
    GState m_current_state;        /// current state of the game
    int m_nextframe;               /// shedule
    CBaseAiAgent *m_p2;            /// P2's AI
    CBaseModel *m_p1_selected[2];  /// P1's last choices
    CBaseModel *m_p2_selected[2];  /// P2's last choices
    int m_p1_score;                /// P1's score
    int m_p2_score;                /// P2's score
};

#endif
