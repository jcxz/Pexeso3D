/*
 * Copyright (C) 2012-2013 Josef Rudolf <xrudol04@stud.fit.vutbr.cz>
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
 * Turn Manager implementation
 */

#include "CTurnManager.h"



/**
 * Constructor
 */
CTurnManager::CTurnManager(CBaseAiAgent *p2) {
  m_p2 = p2;
  init();
}

/**
 * Initialization method.
 */
void CTurnManager::init() {
  m_game_over = false;
  m_nextframe = -1;
  m_p1_score = 0;
  m_p2_score = 0;
  m_current_state = GS_P1_1;
}

/**
 * Returns true if m1 and m2 have equal texture indeX;
 */
bool CTurnManager::endTurn(CBaseModel *m1, CBaseModel *m2) {
  bool retvalue = false;
  //deselect both
  m1->setSelected(false);
  m2->setSelected(false);
  //if they are equal, delete them
  qDebug() << "CTurnManager::endTurn: Comparing" << m1->getID() << " and " << m2->getID() << " with textures " << m1->getTextureIndex() << " and " << m2->getTextureIndex();
  if (m1->getTextureIndex() == m2->getTextureIndex()) {
    qDebug() << "CTurnManager::endTurn: deleting models";
    m_p2->delModel(m1);
    m1->setDeleted(true);
    m_p2->delModel(m2);
    m2->setDeleted(true);
    retvalue = true;
  }
  //if they are not equal, just cover them again
  else {
    m1->setCovered(true);
    m2->setCovered(true);
    retvalue = false;
  }
  if ((m_game_over = m_p2->gameOver()) == true) {
    m_current_state = GS_OVER;
  }
  return retvalue;
}

/**
 * Method which is supposed to be called when 1st player selected a model
 */
void CTurnManager::setModel(CBaseModel *model) {
  //only 2 game states react to this event
  switch (m_current_state) {
    case GS_P1_1:
      qDebug() << "CTurnManager::setModel: GS_P1_1";
      //uncover
      model->setCovered(false);
      //let the ai of player2 attempt to remember this model
      m_p2->setModel(model);
      //store the selected model into a temp variable
      m_p1_selected[0] = model;
      m_current_state = GS_P1_2;
      break;
    case GS_P1_2:
      if (model == m_p1_selected[0]) {
        break;
      }
      qDebug() << "CTurnManager::setModel: GS_P1_2";
      //uncover
      model->setCovered(false);
      //let the ai of player2 attempt to remember this model
      m_p2->setModel(model);
      //store the selected model into a temp variable
      m_p1_selected[1] = model;
      m_current_state = GS_P1_W0;
      break;
    default:
      break;
  }
}

/**
 * Method which is supposed to be called before rendering of each frame.
 * Handles AI's turn and delays.
 * Returns true is game is over.
 */
bool CTurnManager::event(int framenumber) {
  CBaseModel *model = NULL;
  switch (m_current_state) {
    //waiting for player2's first choice
    case GS_P2_1:
      //try if ai returns anything for this frame
      if ((model = m_p2->getModel(framenumber)) == NULL) {
        //if not, just break
        break;
      }
      qDebug() << "CTurnManager::event: returned first model" << model->getID();
      //uncover the chosen model
      model->setCovered(false);
      //let the ai of player2 attempt to remember this model
      m_p2->setModel(model);
      //store the selected model into a temp variable
      m_p2_selected[0] = model;
      m_current_state = GS_P2_2;
      break;
    //waiting for player2's second choice
    case GS_P2_2:
      //try if ai returns anything for this frame
      if ((model = m_p2->getModel(framenumber)) == NULL) {
        break;
      }
      qDebug() << "CTurnManager::event: returned second model" << model->getID();
      //uncover the chosen model
      model->setCovered(false);
      //let the ai of player2 attempt to remember this model
      m_p2->setModel(model);
      //store the selected model into a temp variable
      m_p2_selected[1] = model;
      //shedule evaluation with a delay so that player 1 can examine the choice
      //this delay is longer so that player 1 has time to find the uncovered cubes in a complex scene
      m_nextframe = framenumber + 30;
      m_current_state = GS_P2_W;
      break;
    //shedule delay so that player 1 can examine what he uncovered
    case GS_P1_W0:
      //shedule evaluation of the two chosen models chosen by player1
      m_nextframe = framenumber + 30;
      m_current_state = GS_P1_W;
      break;
    case GS_P1_W:
      //on the scheduled frame...
      if (framenumber == m_nextframe) {
        //deshedule
        m_nextframe = -1;
        //evaluate and on success increase score
        if (endTurn(m_p1_selected[0],m_p1_selected[1])) {
          m_p1_score++;
          qDebug() << "CTurnManager::event: Player1's score incremented to "<< m_p1_score;
          if (m_current_state != GS_OVER) {
            m_current_state = GS_P1_1;
          }
        }
        else {
          if (m_current_state != GS_OVER) {
            m_current_state = GS_P2_1;
          }
        }

      }
      break;
    case GS_P2_W:
      //on the sheduled frame...
      if (framenumber == m_nextframe) {
        m_nextframe = -1;
        //evaluate and on success increase score
        if (endTurn(m_p2_selected[0],m_p2_selected[1])) {
          m_p2_score++;
          qDebug() << "CTurnManager::event: Player2's score incremented to "<< m_p2_score;
          if (m_current_state != GS_OVER) {
            m_current_state = GS_P2_1;
          }
        }
        else {
          if (m_current_state != GS_OVER) {
            m_current_state = GS_P1_1;
          }
        }
      }
      break;
    default:
      break;
  }
  return m_game_over;
}
