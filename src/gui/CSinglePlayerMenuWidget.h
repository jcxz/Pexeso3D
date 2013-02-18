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
 * A widget to display multiplayer menu
 */

#ifndef CSINGLEPLAYERMENUWIDGET_H
#define CSINGLEPLAYERMENUWIDGET_H

#include <QLabel>

#include "CStyledWidget.h"
#include "CSceneBrowser.h"
#include "CPexesoScene.h"


class QPushButton;
class CSelectionBox;
class CImageWidget;

/**
 */
class CSinglePlayerMenuWidget : public CStyledWidget
{
  Q_OBJECT

  public:
    /**
     * Constructor
     */
    explicit CSinglePlayerMenuWidget(QWidget *parent = 0);

    /**
     * This method will return the scene settings
     *
     * @return the current scene name
     */
    QString getSelectedScene(void) const
    {
      return m_browser->getSelectedScene();
    }

    /**
     * This method will return the currently selected difficulty level
     *
     * @return the current difficulty level
     */
    CPexesoScene::EDifficulty getSelectedDifficulty(void) const;

  public slots:
    /**
     * This method will display a given error message on screen
     */
    void setError(const QString & error)
    {
      m_error->setText(error);
      m_error->setVisible(true);
      return;
    }

    /**
     * This method will clear an error message from the screen
     */
    void clearError(void)
    {
      m_error->setVisible(false);
      return;
    }

  signals:
    void startGameTriggered(void);
    void backTriggered(void);

  private:
    QLabel *m_header;             /// a banner on the top of the screen
    QPushButton *m_pb_start;      /// start button
    QPushButton *m_pb_back;       /// back button
    CSelectionBox *m_difficulty;  /// difficulty selections
    //CSelectionBox *m_scene;       /// scene selections
    //CSelectionBox *m_img_set;     /// image set options
    CSceneBrowser *m_browser;     /// a widget to pick the scene to play
    QLabel *m_error;              /// a label to holad an error message
};

#endif
