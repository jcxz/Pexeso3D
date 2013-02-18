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
 * Game end screen implementation
 */

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include "CGameOverWidget.h"



/**
 */
CGameOverWidget::CGameOverWidget(EType type, QWidget *parent)
  : CStyledWidget(parent),
    m_type(type),
    m_caption(0),
    m_message(0),
    m_edit_score(0),
    m_continue(0)
{
  /* create widgets */
  m_caption = new QLabel(tr("Victory"));
  m_caption->setObjectName("m_caption");

  m_message = new QLabel(tr("Press continue to go back to main menu"));
  m_message->setObjectName("m_message");

  m_edit_score = new QLineEdit(tr("Player 1"));
  m_edit_score->setObjectName("m_edit_score");

  m_continue = new QPushButton(tr("Continue"));
  m_continue->setObjectName("m_continue");
  connect(m_continue, SIGNAL(clicked()), SIGNAL(continueTriggered()));

  /* create layout */
  QVBoxLayout *l_main = new QVBoxLayout;
  l_main->addWidget(m_caption, 0, Qt::AlignCenter);
  l_main->addWidget(m_message, 0, Qt::AlignCenter);
  l_main->addWidget(m_edit_score, 0, Qt::AlignCenter);
  l_main->addWidget(m_continue, 0, Qt::AlignCenter);

  setLayout(l_main);
}


/**
 */
void CGameOverWidget::showEvent(QShowEvent *event)
{
  switch (m_type)
  {
    case TYPE_VICTORY:
      m_caption->setText(tr("Victory"));
      m_message->setText(tr("Press continue to go back to main menu"));
      //m_edit_score->setText(tr("Player 1"));
      m_edit_score->setVisible(false);
      break;

    case TYPE_HIGH_SCORE:
      m_caption->setText(tr("Victory"));
      m_message->setText(tr("You won and reached the highest score. "
                            "Please type your name to enter "
                            "the hall of fame and hit continue"));
      m_edit_score->setText(tr("Player 1"));
      m_edit_score->setVisible(true);
      break;

    case TYPE_DEFEAT:
      m_caption->setText(tr("Defeat"));
      m_message->setText(tr("Press continue to go back to main menu"));
      //m_edit_score->setText(tr("Player 1"));
      m_edit_score->setVisible(false);
      break;
  }

  CStyledWidget::showEvent(event);

  return;
}
