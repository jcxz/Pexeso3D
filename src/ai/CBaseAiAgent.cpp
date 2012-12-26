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
 * This implements CPU oponent's artificial intelligence
 */

#include "CBaseAiAgent.h"


const int CBaseAiAgent::m_skillpercent[] = {25,50,75};



/**
 * Constructor
 */
CBaseAiAgent::CBaseAiAgent(QVector<CBaseModel *> *modelptr, ESkill skill) {
  m_mptr = modelptr;
  m_skill = skill;
  m_nextframe = -1; //-1 means nothing is sheduled
  m_response1 = NULL;
  m_response2 = NULL;
  m_one_choice = false;
}

/**
 * Method which copies all model pointers to a local QVector.
 * It has to be done separately from Constructor because it does not work before the scene is loaded.
 */
void CBaseAiAgent::initModels() {
  m_alive = *m_mptr;
  qDebug() << m_alive.size();
}

/**
 * A Model's texture has been uncovered. This method stores it into ai's memory with a probability based on game difficulty.
 */
void CBaseAiAgent::setModel(CBaseModel *model) {
  bool exists = false; //a bool to temporarily store whether the model is already in ai's memory.
  if ((rand()%100) < m_skillpercent[m_skill]) { //if random number falls into the percentage limit
    for(QVector<CBaseModel *>::iterator i=m_known.begin(); i != m_known.end(); i++) {
      //iterate through all known models and compare
      if (*i == model) {
        exists = true;
      }
    }
    if (!exists) {
      //if it doesn't exist in ai's memory yet, add it.
      qDebug() << "CBaseAiAgent::setModel: remembering...";
      m_known.push_back(model); 
    }
    else {
      qDebug() << "CBaseAiAgent::setModel: already exists";
    }
  }
  else {
    qDebug() << "CBaseAiAgent::setModel: did not remember";
  }
//   m_models.append(model);
}

/**
 * Choose two models which the ai will uncover in this turn.
 */
void CBaseAiAgent::generateResponse() {
  m_response1 = NULL;
  m_response2 = NULL;
  bool chosen = false;
  //temporary integer
  int num1 = 0;
  //one has already been chosen, select the second one
  if (m_one_choice) {
    qDebug() << "CBaseAiAgent::generateResponse: Entering m_one_choice branch.";
  //if random number falls into the percentage limit, try to find a match in memory
    if ((rand()%100) < m_skillpercent[m_skill]) {
      qDebug() << "CBaseAiAgent::generateResponse: Attempting to find a matching second one.";
      for(QVector<CBaseModel *>::iterator i=m_known.begin(); i != m_known.end(); i++) {
        if (((*i)->getID() != m_oc->getID()) && ((*i)->getTextureIndex() == m_oc->getTextureIndex())) {
          m_response1 = NULL;
          m_response2 = *i;
          qDebug() << "CBaseAiAgent::generateResponse: A matching second one chosen." << m_response2->getID();
          chosen = true;
          break;
        }
      }
    }
    //if no chosen, choose randomly
    if (!chosen) {
      do {
        num1 = rand() % (m_alive.size());
//         qDebug() << "CBaseAiAgent::generateResponse: " << num1;
      } while (m_alive[num1]->getID() == m_oc->getID());
      m_response1 = NULL;
      m_response2 = m_alive[num1];
      qDebug() << "CBaseAiAgent::generateResponse: Randomly chosen second one" << m_response2->getID();
    }
  }
  //try to first one
  else {
    qDebug() << "CBaseAiAgent::generateResponse: Entering the normal branch.";
    //if random number falls into the percentage limit, try to find a matching pair in ai's memory
    if ((rand()%100) < m_skillpercent[m_skill]) {
      qDebug() << "CBaseAiAgent::generateResponse: Attempting to choose a matching pair.";
      //find a pair in memory
      for(QVector<CBaseModel *>::iterator i=m_known.begin(); i != m_known.end(); i++) {
        for(QVector<CBaseModel *>::iterator j=m_known.begin(); j != m_known.end(); j++) {
          if ((*i)->getID() != (*j)->getID() && (*i)->getTextureIndex() == (*j)->getTextureIndex()) {
            m_response1 = *i;
            m_response2 = *j;
            qDebug() << "CBaseAiAgent::generateResponse: A matching pair chosen.";
            chosen = true;
            m_one_choice = false;
            break;
          }
        }
        if (chosen) {
          break;
        }
      }
    }
    
    //if random number fell outside percentage limit or no matching pair in memory was found,
    //just randomly choose one model
    if (m_response1 == NULL || m_response2 == NULL) {
      
      //generate number in the range of active models
      num1 = rand() % (m_alive.size());
      
      m_response1 = m_alive[num1];
      m_response2 = NULL;
      m_oc = m_response1;
      m_one_choice = true;
      
      qDebug() << "CBaseAiAgent::generateResponse: Randomly chosen " << m_alive[num1]->getID();
    }
  }
}

/**
 * Method which returns a chosen model on the sheduled frame.
 */
CBaseModel *CBaseAiAgent::getModel(int frame) {
  CBaseModel *retvalue = NULL;
  //nothing has been sheduled yet, choose two models and schedule return of the first one.
  if (m_nextframe == -1) {
    qDebug() << frame << " CBaseAiAgent::getModel: generating a random model";
    generateResponse();
    m_nextframe = frame + 15;
  }
  //on the sheduled frame,...
  else if (frame == m_nextframe) {
    //if only one has been chosen so far
    if (m_one_choice) {
      retvalue = m_response1;
      //generate the second one
      generateResponse();
      //clear the one-choice flag
      m_one_choice = false;
      qDebug() << frame << " CBaseAiAgent::getModel: returning first model of one-choice " << retvalue->getID();
      m_nextframe = frame + 15;
    }
    //both chosen at once
    else {
      //return the first model and schedule return of the second one
      if (m_response1 != NULL) {
        retvalue = m_response1;
        qDebug() << frame << " CBaseAiAgent::getModel: returning first model" << retvalue->getID();
        m_response1 = NULL;
        m_nextframe = frame + 15;
      }
      //return the second one
      else {
        if (m_response2 == NULL) {
          qDebug() << frame << " CBaseAiAgent::getModel: generating two models when 2nd should be already set.";
          generateResponse();
          m_nextframe = frame + 15;
        }
        retvalue = m_response2;
        qDebug() << frame << "CBaseAiAgent::getModel: returning second model" << retvalue->getID();
        m_response2 = NULL;
        m_nextframe = -1;
      }
    }
  }
  return retvalue;
}

/**
 * Method to remove a model from ai's memory if it has been set as deleted.
 */
//void *CBaseAiAgent::delModel(CBaseModel *model) {
void CBaseAiAgent::delModel(CBaseModel *model) {
  for(QVector<CBaseModel *>::iterator i=m_alive.begin(); i != m_alive.end(); i++) {
    if (*i == model) {
      m_alive.erase(i);
      break;
    }
  }
  for(QVector<CBaseModel *>::iterator i=m_known.begin(); i != m_known.end(); i++) {
    if (*i == model) {
      m_known.erase(i);
      break;
    }
  }
}

/**
 * Method which returns wether all models have been deleted.
 */
bool CBaseAiAgent::gameOver() {
  return (m_alive.size() == 0);
}
