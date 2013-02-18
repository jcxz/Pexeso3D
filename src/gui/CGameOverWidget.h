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
 * Screen at the end of the game
 */

#ifndef CGAMEOVERWIDGET_H
#define CGAMEOVERWIDGET_H

#include "CStyledWidget.h"


class QLabel;
class QLineEdit;
class QPushButton;

/**
 */
class CGameOverWidget : public CStyledWidget
{
  Q_OBJECT

  public:
    /** This enumeration defines the type of game over screen */
    enum EType {
      TYPE_VICTORY = 0,       /// a victory screen
      TYPE_HIGH_SCORE,        /// a screen to enter the highest score
      TYPE_DEFEAT             /// a defeat screen
    };

  public:
    explicit CGameOverWidget(EType type = TYPE_VICTORY, QWidget *parent = 0);

    /**
     * This function will return the currently set type of the screen
     */
    EType getType(void) const
    {
      return m_type;
    }

    /**
     * This method will set the type of the screen
     */
    void setType(EType type)
    {
      m_type = type;
      return;
    }

  signals:
    void continueTriggered(void);

  protected:
    virtual void showEvent(QShowEvent *event);

  private:
    EType m_type;             /// the type of the screen
    QLabel *m_caption;        /// a widget caption
    QLabel *m_message;        /// an additional message for the player
    QLineEdit *m_edit_score;  /// an input box for score
    QPushButton *m_continue;  /// to go back to main menu
};

#endif
