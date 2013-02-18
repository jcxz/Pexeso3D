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
 * Main menu screen implementation
 */

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>

#include "CMainMenuWidget.h"
#include "CImageWidget.h"
#include "CCreditsLabel.h"



/**
 */
CMainMenuWidget::CMainMenuWidget(QWidget *parent)
  : CStyledWidget(parent),
    m_pb_single_player(0),
    m_pb_multi_player(0),
    m_pb_highscores(0),
    m_pb_exit(0),
    m_credits(0),
    m_logo(0)
{
  /* set up buttons */
  m_pb_single_player = new QPushButton(tr("Single Player"));
  m_pb_single_player->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  connect(m_pb_single_player, SIGNAL(clicked()), SIGNAL(singlePlayerTriggered()));

  m_pb_multi_player = new QPushButton(tr("Multi Player"));
  m_pb_multi_player->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  connect(m_pb_multi_player, SIGNAL(clicked()), SIGNAL(multiPlayerTriggered()));

  m_pb_highscores = new QPushButton(tr("Highest scores"));
  m_pb_highscores->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
  connect(m_pb_highscores, SIGNAL(clicked()), SIGNAL(highScoresTriggered()));

  m_pb_exit = new QPushButton(tr("Quit Game"));
  m_pb_exit->setObjectName("m_pb_exit");
  //m_pb_highscores->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
  connect(m_pb_exit, SIGNAL(clicked()), SIGNAL(exitTriggered()));

  /* set up logo and credits label */
  m_credits = new CCreditsLabel;
  m_logo = new CImageWidget(":/bin/gui/images/logo.png", 300, 300);// Qt::KeepAspectRatio);

  /* create layouts */
  QGridLayout *l_credits_exit = new QGridLayout;
  l_credits_exit->addWidget(m_pb_exit, 0, 0, Qt::AlignLeft);
  l_credits_exit->addWidget(m_credits, 0, 1, Qt::AlignRight);

  QGridLayout *l_widgets = new QGridLayout;
  //l_widgets->addWidget(m_logo, 0, 0, 1, 0, Qt::AlignCenter);
  l_widgets->addWidget(m_pb_single_player, 1, 0, 1, 0, Qt::AlignCenter);
  l_widgets->addWidget(m_pb_multi_player, 2, 0, 1, 0, Qt::AlignCenter);
  l_widgets->addWidget(m_pb_highscores, 3, 0, 1, 0, Qt::AlignCenter);

  QVBoxLayout *l_main = new QVBoxLayout;
  l_main->setMargin(20);   // make 20 pixel margins
  l_main->addStretch();
  l_main->addWidget(m_logo, 0, Qt::AlignCenter);
  l_main->addStretch();
  l_main->addLayout(l_widgets);
  l_main->addStretch();
  l_main->addLayout(l_credits_exit);

  setLayout(l_main);
}
