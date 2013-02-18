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
 * Implements single player menu widget
 */

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "CSinglePlayerMenuWidget.h"
#include "CImageWidget.h"
#include "CSelectionBox.h"
#include "CSceneBrowser.h"



/**
 */
CSinglePlayerMenuWidget::CSinglePlayerMenuWidget(QWidget *parent)
  : CStyledWidget(parent),
    m_pb_start(0),
    m_pb_back(0),
    m_difficulty(0),
    //m_scene(0),
    //m_img_set(0),
    m_browser(0),
    m_error(0)
{
  /* create widgets */
  m_header = new QLabel("New Game");
  m_header->setObjectName("m_header_label");

  m_difficulty = new CSelectionBox(tr("Select difficulty"));
  m_difficulty->addOption(tr("Easy"));
  m_difficulty->addOption(tr("Medium"));
  m_difficulty->addOption(tr("Hard"));
  m_difficulty->setCurrentOption(0);

  //m_scene = new CSelectionBox(tr("Select scene"));
  //m_img_set = new CSelectionBox(tr("Select image set"));
  m_browser = new CSceneBrowser(tr("Select scene"));
  //m_browser->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  m_browser->setSceneListFile(g_scene_list_file);

  m_pb_start = new QPushButton(tr("Start Game"));
  m_pb_start->setObjectName("m_pb_start");
  connect(m_pb_start, SIGNAL(clicked()), SIGNAL(startGameTriggered()));

  m_pb_back = new QPushButton(tr("Back"));
  m_pb_back->setObjectName("m_pb_back");
  connect(m_pb_back, SIGNAL(clicked()), SIGNAL(backTriggered()));

  m_error = new QLabel(tr("An error occured"));
  m_error->setObjectName("m_error");
  m_error->setAlignment(Qt::AlignCenter);
  m_error->setVisible(false);

  /* create layout */
  QGridLayout *l_start_back = new QGridLayout;
  l_start_back->addWidget(m_pb_back, 0, 0, Qt::AlignLeft);
  l_start_back->addWidget(m_pb_start, 0, 1, Qt::AlignRight);

  QVBoxLayout *l_main = new QVBoxLayout;
  l_main->setMargin(20);   // make 20 pixel margins
  l_main->addStretch();
  l_main->addWidget(m_header, 0, Qt::AlignCenter);
  l_main->addStretch();
  l_main->addWidget(m_difficulty);
  l_main->addWidget(m_browser);
  l_main->addStretch();
  l_main->addWidget(m_error);
  l_main->addLayout(l_start_back);

  setLayout(l_main);
}


/**
 */
CPexesoScene::EDifficulty CSinglePlayerMenuWidget::getSelectedDifficulty(void) const
{
  if (m_difficulty->getCurrentOption() == tr("Easy"))
  {
    return CPexesoScene::EASY;
  }
  else if (m_difficulty->getCurrentOption() == tr("Medium"))
  {
    return CPexesoScene::MEDIUM;
  }
  else if (m_difficulty->getCurrentOption() == tr("Hard"))
  {
    return CPexesoScene::HARD;
  }

  return CPexesoScene::MEDIUM;
}
