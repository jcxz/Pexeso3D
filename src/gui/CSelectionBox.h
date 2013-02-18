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
 * A widget that allows browsing between various options
 */

#ifndef CSELECTIONBOX_H
#define CSELECTIONBOX_H

#include <QLabel>

#include "CStyledWidget.h"


class QPushButton;

/**
 * This class represents a combo box,
 * where options can be scroled by clicking
 * on a left or rightbutton
 */
class CSelectionBox : public CStyledWidget
{
  Q_OBJECT

  public:
    /**
     * Constructor
     */
    explicit CSelectionBox(const QString & caption = QString(), QWidget *parent = 0);

    /**
     * This method will retrieve the widget's caption
     */
    QString getCaption(void) const
    {
      return m_caption->text();
    }

    /**
     * This method will set the widgets caption
     */
    void setCaption(const QString & caption)
    {
      m_caption->setText(caption);
      return;
    }

    /**
     * This method will return the currently selected option.
     * If no option is selected an empty string will be returned.
     */
    QString getCurrentOption(void) const
    {
      if (m_cur_opt_ind < 0)
      {
        return QString();
      }

      return m_opts[m_cur_opt_ind];
    }

    /**
     * This method will get the index of currently selected
     * option. An index of -1 means that no option is selected.
     */
    int getCurrentOptionIndex(void) const
    {
      return m_cur_opt_ind;
    }

    /**
     * This method will set the option at index i as the current option
     */
    void setCurrentOption(int i)
    {
      if ((i >= 0) && (i < m_opts.size()))
      {
        m_cur_opt_ind = i;
        m_cur_opt->setText(m_opts[i]);
      }

      return;
    }

    /**
     * This method will add a new option to the widget
     */
    void addOption(const QString & option)
    {
      m_opts.append(option);
      return;
    }

  private slots:
    void handlePrevBtnClicked(void);
    void handleNextBtnClicked(void);

  private:
    QLabel *m_caption;       /// caption
    QLabel *m_cur_opt;       /// the currently displayed option
    QPushButton *m_pb_prev;  /// a button to the previous option
    QPushButton *m_pb_next;  /// a button to the next option
    int m_cur_opt_ind;       /// the index of currently diplayed option
    QList<QString> m_opts;   /// a list of options
};

#endif
