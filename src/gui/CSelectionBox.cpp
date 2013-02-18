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
 * Implements selection box widget
 */

#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "CSelectionBox.h"



/**
 */
CSelectionBox::CSelectionBox(const QString & caption, QWidget *parent)
  : CStyledWidget(parent),
    m_caption(0),
    m_cur_opt(0),
    m_pb_prev(0),
    m_pb_next(0),
    m_cur_opt_ind(-1),
    m_opts()
{
  /* create widgets */
  m_caption = new QLabel(caption);
  m_cur_opt = new QLabel;
  m_caption->setObjectName("caption");

  m_pb_prev = new QPushButton();
  m_pb_prev->setObjectName("m_pb_prev");
  connect(m_pb_prev, SIGNAL(clicked()), SLOT(handlePrevBtnClicked()));

  m_pb_next = new QPushButton();
  m_pb_next->setObjectName("m_pb_next");
  connect(m_pb_next, SIGNAL(clicked()), SLOT(handleNextBtnClicked()));

  /* set up layout */
  QHBoxLayout *l_widgets = new QHBoxLayout;
  l_widgets->addWidget(m_pb_prev);
  l_widgets->addStretch();
  l_widgets->addWidget(m_cur_opt);
  l_widgets->addStretch();
  l_widgets->addWidget(m_pb_next);

  QVBoxLayout *l_main = new QVBoxLayout;
  l_main->addWidget(m_caption, 0, Qt::AlignCenter);
  l_main->addLayout(l_widgets);

  setLayout(l_main);
}


/**
 */
void CSelectionBox::handlePrevBtnClicked(void)
{
  int size = m_opts.size();
  if (size > 0)
  {
    m_cur_opt_ind = (m_cur_opt_ind + size - 1) % size;
    m_cur_opt->setText(m_opts[m_cur_opt_ind]);
  }

  return;
}


/**
 */
void CSelectionBox::handleNextBtnClicked(void)
{
  int size = m_opts.size();
  if (size > 0)
  {
    m_cur_opt_ind = (m_cur_opt_ind + 1) % size;
    m_cur_opt->setText(m_opts[m_cur_opt_ind]);
  }

  return;
}
