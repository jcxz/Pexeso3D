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
 * Convenience widget to display images
 */

#ifndef CIMAGEWIDGET_H
#define CIMAGEWIDGET_H

#include <QLabel>
#include <QPixmap>


/**
 * This class provides a widget to display
 * an image. It is subclassed only for easier styling
 * with stylsheets.
 */
class CImageWidget : public QLabel
{
  Q_OBJECT

  public:
    /**
     * Default constructor
     */
    // TODO: maybe remove
    explicit CImageWidget(QWidget *parent = 0)
      : QLabel(parent)
    {
    }

    /**
     * Construct widget using a given pixmap
     */
    CImageWidget(const QPixmap & pixmap,
                 int w_max,
                 int h_max,
                 Qt::AspectRatioMode aspect_mode = Qt::IgnoreAspectRatio,
                 QWidget *parent = 0)
      : QLabel(parent)
    {
      setMaximumSize(w_max, h_max);
      setPixmap(pixmap.scaled(QSize(w_max, h_max),
                              aspect_mode,
                              Qt::SmoothTransformation));  // use smooth transformation otherwise the image might appear jaggy
    }

    /**
     * Construct a widget by loading the pixmap from a given file
     */
    CImageWidget(const QString & img_path,
                 int w_max,
                 int h_max,
                 Qt::AspectRatioMode aspect_mode = Qt::IgnoreAspectRatio,
                 QWidget *parent = 0)
      : QLabel(parent)
    {
      setMaximumSize(w_max, h_max);
      setPixmap(QPixmap(img_path).scaled(QSize(w_max, h_max),
                                         aspect_mode,
                                         Qt::SmoothTransformation));  // same here
    }
};

#endif
