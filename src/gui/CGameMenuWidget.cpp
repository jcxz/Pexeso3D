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
 * Game menu screen implementation
 */

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

#include "CGameMenuWidget.h"



/**
 */
CGameMenuWidget::CGameMenuWidget(QWidget *parent)
  : CStyledWidget(parent),
    m_banner(0),
    m_pb_resume(0),
    m_pb_main_menu(0),
    m_pb_quit(0)
{
  /* create widgets */
  m_banner = new QLabel(tr("Game menu"));
  m_banner->setObjectName("m_banner");

  m_pb_main_menu = new QPushButton(tr("Main Menu"));
  m_pb_main_menu->setObjectName("m_pb_main_menu");
  connect(m_pb_main_menu, SIGNAL(clicked()), SIGNAL(mainMenuTriggered()));

  m_pb_quit = new QPushButton(tr("Quit"));
  m_pb_quit->setObjectName("m_pb_quit");
  connect(m_pb_quit, SIGNAL(clicked()), SIGNAL(quitTriggered()));

  m_pb_resume = new QPushButton(tr("Resume"));
  m_pb_resume->setObjectName("m_pb_resume");
  connect(m_pb_resume, SIGNAL(clicked()), SIGNAL(resumeTriggered()));

  /* create layout */
  QVBoxLayout *l_menu = new QVBoxLayout;
  l_menu->addWidget(m_banner, 0, Qt::AlignCenter);
  l_menu->addWidget(m_pb_resume, 0, Qt::AlignCenter);
  l_menu->addWidget(m_pb_main_menu, 0, Qt::AlignCenter);
  l_menu->addWidget(m_pb_quit, 0, Qt::AlignCenter);
  l_menu->setSizeConstraint(QLayout::SetFixedSize);

  setLayout(l_menu);
}
