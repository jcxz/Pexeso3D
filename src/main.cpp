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
 * Contains main() function
 */

#include <QApplication>
#include <QDesktopWidget>
#include <QGLWidget>
#include <QMessageBox>
#include <QDebug>
#include <QFile>

#include "Global.h"
#include "ogl.h"

//#define PEXESO_NO_MENU

#ifdef PEXESO_NO_MENU
# include "CPexesoScene.h"
# include "CPexesoView.h"
#else
# include "CMainWindow.h"
#endif


/**
 * This function will initialize GLEW and
 * make sure that all required extensions
 * are supported
 */
#if 0
static bool checkGLExtensions(void)
{
  GLenum error = glewInit();
  if (error != GLEW_OK)
  {
    qCritical() << "Failed to initialize GLEW: " << glewGetErrorString(error);
    return false;
  }

  if (!GLEW_VERSION_1_4)
  {
    qCritical() << "OpenGL 1.3 or higher required. Update your graphics card driver";
    return false;
  }

  return true;
}
#endif



////////////////////
//// Main Program
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  /* command line arguments parsing will come here and loading settings */

  /* initialize OpenGL and extensions */
  if ((QGLFormat::openGLVersionFlags() & QGLFormat::OpenGL_Version_1_5) == 0)
  {
    QMessageBox::critical(0, g_app_name, QObject::tr("OpenGL version 1.5 or higher is "
                                                     "required to run this demo.\n"
                                                     "The program will now exit."));
    return 1;
  }

  /* use sample buffers to get antialiasing ??? */
  //QGLWidget *glwidget = new QGLWidget(QGLFormat(QGL::SampleBuffers));
  QGLWidget *glwidget = new QGLWidget();
  //QGLWidget *glwidget = new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::NoStencilBuffer));

  glwidget->makeCurrent();
  if (!OpenGL::SExtensions::instance.initExtensions(glwidget->context()))
  {
    QMessageBox::critical(0, g_app_name, QObject::tr("Some of the required OpenGL "
                                                     "extensions were not resolved"));
    return 1;
  }

  /* output an information on suported image formats */
  qDebug() << QImageReader::supportedImageFormats();

  /* load stylesheets (according to settings) */
  {
    QFile file(g_qss_file);

    if (!file.open(QIODevice::ReadOnly))
    {
      QMessageBox::warning(0, g_app_name, QObject::tr("Failed to load stylesheet file.\n"
                                                      "Falling back to default style."));
    }

    QString style(file.readAll());
    qApp->setStyleSheet(style);     // qApp is the same as QApplication::instance()

    //file.close();  // to release the OS lock on style file
  }

#ifdef PEXESO_NO_MENU
  CPexesoScene scene;
  CPexesoView view(glwidget, &scene);

  QObject::connect(&scene, SIGNAL(gameOverTriggered()), &view, SLOT(close()));
  QObject::connect(&scene, SIGNAL(quitTriggered()), &view, SLOT(close()));
  QObject::connect(&scene, SIGNAL(errorTriggered()), &view, SLOT(close()));

  glwidget->makeCurrent();
  if (!scene.newGame(argv[1]))
  {
    qDebug() << "Failed to start a new game: " << scene.getErrorString();
    return 1;
  }

  view.show();

#else

  /* show the application window and run it */
  CMainWindow window(glwidget);
  QString arg1(argv[1]);

  if (arg1 == "--fullscreen")
  { // fullscreen mode
    window.showFullScreen();
  }
  else if (arg1 == "--maximized")
  { // maximized mode
    window.showMaximized();
  }
  else
  { // normal mode
    QDesktopWidget desktop;
    QRect desktop_rect = desktop.screenGeometry();
    int window_w = 1024;
    int window_h = 640;

    window.setGeometry((desktop_rect.width() - window_w) / 4.0f,
                       (desktop_rect.height() - window_h) / 4.0f,
                       window_w,
                       window_h);

    window.show();
  }
#endif

  return app.exec();
}
