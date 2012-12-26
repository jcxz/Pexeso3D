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
 * Implements available scene browser
 */

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "CSceneBrowser.h"



/**
 */
CSceneBrowser::CSceneBrowser(const QString & caption, QWidget *parent)
  : CStyledWidget(parent),
    m_pb_prev(0),
    m_pb_next(0),
    m_caption(0),
    m_scene_name(0),
    m_scene_desc(0),
    m_cur_scene(-1),
    m_scenes()
{
  /* create widgets */
  m_caption = new QLabel(caption);
  m_caption->setAlignment(Qt::AlignCenter);
  m_caption->setObjectName("caption");

  m_scene_name = new QLabel;
  m_scene_name->setAlignment(Qt::AlignCenter);

  m_scene_desc = new QLabel;
  m_scene_desc->setWordWrap(true);
  m_scene_desc->setAlignment(Qt::AlignCenter);
  m_scene_desc->setObjectName("desc");

  m_pb_prev = new QPushButton;
  m_pb_prev->setObjectName("m_pb_prev");
  connect(m_pb_prev, SIGNAL(clicked()), SLOT(handlePrevBtnClicked()));

  m_pb_next = new QPushButton;
  m_pb_next->setObjectName("m_pb_next");
  connect(m_pb_next, SIGNAL(clicked()), SLOT(handleNextBtnClicked()));

  /* set up layout */
  QVBoxLayout *l_widgets = new QVBoxLayout;
  l_widgets->addWidget(m_caption);
  l_widgets->addStretch();
  l_widgets->addWidget(m_scene_name);
  l_widgets->addStretch();
  l_widgets->addWidget(m_scene_desc);

  QHBoxLayout * l_main = new QHBoxLayout;
  l_main->addWidget(m_pb_prev);
  l_main->addStretch();
  l_main->addLayout(l_widgets);
  l_main->addStretch();
  l_main->addWidget(m_pb_next);

  //setStyleSheet("CStyledWidget { background-color : red }");
  setLayout(l_main);
}


/**
 */
void CSceneBrowser::showEvent(QShowEvent *event)
{
  qDebug() << "reloading";

  CScene::EErrorCode err = CScene::loadSceneList(m_scene_list_file, &m_scenes, true);
  if ((err != CScene::ERR_OK) || (m_scenes.size() <= 0))
  {
    qDebug() << "Failed to refresh scene list";
    m_cur_scene = -1;
    m_scene_name->setText(tr("No scene"));
    m_scene_desc->setText(tr("There are no scenes to be loaded"));
  }
  else
  {
    m_cur_scene = 0;
    const SSceneInfo & info = m_scenes[m_cur_scene];
    m_scene_name->setText(info.name);
    m_scene_desc->setText(info.desc);
  }

  CStyledWidget::showEvent(event);

  return;
}


/**
 */
void CSceneBrowser::handlePrevBtnClicked(void)
{
  int size = m_scenes.size();
  if (size > 0)
  {
    m_cur_scene = (m_cur_scene + size - 1) % size;
    const SSceneInfo & info = m_scenes[m_cur_scene];
    m_scene_name->setText(info.name);
    m_scene_desc->setText(info.desc);
  }

  return;
}


/**
 */
void CSceneBrowser::handleNextBtnClicked(void)
{
  int size = m_scenes.size();
  if (size > 0)
  {
    m_cur_scene = (m_cur_scene + 1) % size;
    const SSceneInfo & info = m_scenes[m_cur_scene];
    m_scene_name->setText(info.name);
    m_scene_desc->setText(info.desc);
  }

  return;
}
