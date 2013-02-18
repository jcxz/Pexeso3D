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
 * A widget that allows browsing between available scenes
 */

#ifndef CSCENEBROWSER_H
#define CSCENEBROWSER_H

#include <QVector>
#include <QLabel>

#include "CStyledWidget.h"
#include "CScene.h"


class QPushButton;

/**
 * This class is a widget to to browse between game scene
 */
class CSceneBrowser : public CStyledWidget
{
  Q_OBJECT

  public:
    explicit CSceneBrowser(const QString & caption = QString(), QWidget *parent = 0);

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
     * This method will reutrn the name of file that is reloaded each
     * time the widget is shown
     */
    const QString & getSceneListFile(void) const
    {
      return m_scene_list_file;
    }

    /**
     * This method will set the scene list file that will be reloaded
     * each time a widget will be shown
     */
    void setSceneListFile(const QString & filename)
    {
      m_scene_list_file = filename;
      return;
    }

    /**
     * This method will return a path to the selected scene,
     * so that it can be loaded.
     *
     * @return a path to the selected scene file, so that it can be loaded
     *         On error an empty string is returned.
     */
    QString getSelectedScene(void) const
    {
      if (m_cur_scene < 0)
      {
        return QString();
      }

      return m_scenes[m_cur_scene].path;
    }

  protected:
    virtual void showEvent(QShowEvent *event);

  private slots:
    void handlePrevBtnClicked(void);
    void handleNextBtnClicked(void);

  private:
    QPushButton *m_pb_prev;        /// a button to browse to previous widget
    QPushButton *m_pb_next;        /// a button to browse to next widget
    QLabel *m_caption;             /// widget's caption
    QLabel *m_scene_name;          /// name of the currently selected scene
    QLabel *m_scene_desc;          /// desc. of the currently selected scene
    int m_cur_scene;               /// the currently selected scene
    QVector<SSceneInfo> m_scenes;  /// a list of available scenes
    QString m_scene_list_file;     /// the file that will be reloaded on widget show event
};

#endif
