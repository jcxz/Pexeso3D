/*
 * Copyright (C) 2012 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>
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
 * Multiplayer menu widget implementation
 */

#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>

#include "CMultiPlayerMenuWidget.h"
#include "CImageWidget.h"



/**
 */
CMultiPlayerMenuWidget::CMultiPlayerMenuWidget(QWidget *parent)
  : CStyledWidget(parent),
    m_pb_join_game(0),
    m_pb_host_game(0),
    m_pb_back(0),
    m_header(0)
{
  /* create buttons */
  m_pb_join_game = new QPushButton(tr("Join game"));
  connect(m_pb_join_game, SIGNAL(clicked()), SIGNAL(joinGameTriggered()));

  m_pb_host_game = new QPushButton(tr("Host game"));
  connect(m_pb_host_game, SIGNAL(clicked()), SIGNAL(hostGameTriggered()));

  m_pb_back = new QPushButton(tr("Back"));
  m_pb_back->setObjectName("m_pb_back");
  connect(m_pb_back, SIGNAL(clicked()), SIGNAL(backTriggered()));

  m_header = new QLabel("Multi player");
  m_header->setObjectName("m_header_label");

  /* create layouts */
  QGridLayout *l_back = new QGridLayout;
  l_back->addWidget(m_pb_back, 0, 0, Qt::AlignLeft);

  QGridLayout *l_widgets = new QGridLayout;
  //l_widgets->addWidget(m_header, 0, 0, 1, 0, Qt::AlignCenter);
  l_widgets->addWidget(m_pb_join_game, 1, 0, 1, 0, Qt::AlignCenter);
  l_widgets->addWidget(m_pb_host_game, 2, 0, 1, 0, Qt::AlignCenter);

  QVBoxLayout *l_main = new QVBoxLayout;
  l_main->setMargin(20);   // make 20 pixel margins
  l_main->addStretch();
  l_main->addWidget(m_header, 0, Qt::AlignCenter);
  l_main->addStretch();
  l_main->addLayout(l_widgets);
  l_main->addStretch();
  l_main->addLayout(l_back);

  setLayout(l_main);
}
