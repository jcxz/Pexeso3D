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
 * This defines CPU oponent's artificial intelligence
 */

#ifndef CBASEAIAGENT_H
#define CBASEAIAGENT_H

#include <QList>

#include "CBaseModel.h"


class CBaseModel;

/**
 * This class represents a base for CPU players
 */
class CBaseAiAgent
{
  public:
    /** This enumeration defines how good an artificial oponent is */
    enum ESkill {
      BEGINNER = 0,   /// the least skilled AI oponent
      ADVANCED,       /// moderately skilled ai opponent
      PROFESSIONAL,   /// highly skilled ai oponent
      SKILL_NULL
    };

  public:
    explicit CBaseAiAgent(QVector<CBaseModel *> *modelptr, ESkill skill = ADVANCED);

    /**
     * This method will return the ai's skill level
     */
    int getSkill(void) const
    {
      return m_skill;
    }

    /**
     * This method will set the ai's skill level
     */
    void setSkill(int skill)
    {
      m_skill = skill;
      return;
    }

    /**
     * A Model's texture has been uncovered. This method stores it into ai's memory with a probability based on game difficulty.
     */
    void setModel(CBaseModel *model);
    
    /**
     * Method which returns a chosen model on the sheduled frame.
     */
    CBaseModel *getModel(int frame);
    
    /**
     * Method to remove a model from ai's memory if it has been set as deleted.
     */
    //void *delModel(CBaseModel *model);
    void delModel(CBaseModel *model);
    
    /**
     * Method which copies all model pointers to a local QVector.
     * It has to be done separately from Constructor because it does not work before the scene is loaded.
     */
    void initModels();
    
    /**
     * Method which returns wether all models have been deleted.
     */
    bool gameOver();

  private:
    /**
     * Choose two models which the ai will uncover in this turn.
     */
    void generateResponse();
    
  protected:
    int m_skill;                        /// how skilled this ai oponent is
    QVector<CBaseModel *> *m_mptr;      /// pointer to the whole CScene's vector of all models
    QVector<CBaseModel *> m_known;      /// vector of models the ai remembered on uncovering
    QVector<CBaseModel *> m_alive;      /// vector of models that haven't been deleted yet
    static const int m_skillpercent[];  /// array of difficulty percentages used when generating pseudo-random numbers
    int m_nextframe;                    /// sheduling frame for delayed operations
    CBaseModel *m_response1;            /// first model to be returned
    CBaseModel *m_response2;            /// second model to be returned
    CBaseModel *m_oc;
    bool m_one_choice;                  /// indicates whether only one block has been chosen so far
};

#endif
