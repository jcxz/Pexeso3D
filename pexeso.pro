################################################################################
#                                                                              #
# Copyright (C) 2012-2013 Matus Fedorko <xfedor01@stud.fit.vutbr.cz>           #
#                                                                              #
# This file is part of Pexeso3D.                                               #
#                                                                              #
# Pexeso3D is free software: you can redistribute it and/or modify             #
# it under the terms of the GNU General Public License as published by         #
# the Free Software Foundation, either version 3 of the License, or            #
# (at your option) any later version.                                          #
#                                                                              #
# Pexeso3D is distributed in the hope that it will be useful,                  #
# but WITHOUT ANY WARRANTY; without even the implied warranty of               #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                #
# GNU General Public License for more details.                                 #
#                                                                              #
# You should have received a copy of the GNU General Public License            #
# along with Pexeso3D.  If not, see <http://www.gnu.org/licenses/>.            #
#                                                                              #
################################################################################

TEMPLATE = app      # project type
TARGET = pexeso     # the application name
DESTDIR = bin       # the destination directory
MOC_DIR = obj       # meta object compoliler directory
RCC_DIR = obj       # a directory for compiled resource files
OBJECTS_DIR = obj   # a directory for temporary compile output
QT += opengl        # include OpenGL
#CONFIG += qt debug

# tweak compilation flags for gcc.
# When you are using a different compiler on unix platform
# just comment this setting out
unix:QMAKE_CXXFLAGS += \
  -std=c++98 \
  -pedantic \
  -Wextra \
  -Wno-long-long # do not complain abount long long in gt headers

#
DEPENDPATH += .

# additional include directories
INCLUDEPATH += \
  . \
  ./src \
  ./src/ai \
  ./src/game \
  ./src/gui \
  ./src/maths

# Additional libraries
#win32: LIBS += $$OUT_PWD/libs/glew/lib/glew32s.lib  # the glew32s.lib version is for static linkage
#unix: LIBS += -L$$OUT_PWD/libs/glew/lib -lGLEW -lGLU
unix: LIBS += -lGLU


# Header files
HEADERS += \
  src/maths/Maths.h \
  src/ai/CBaseAiAgent.h \
  src/maths/SVector3D.h \
  src/maths/SVector2D.h \
  src/game/ogl.h \
  src/game/CTextureStore.h \
  src/game/CIndexBuffer.h \
  src/game/CVertexBuffer.h \
  src/game/CBBox.h \
  src/game/CBaseModel.h \
  src/game/CCubeModel.h \
  src/game/CTrackBall.h \
  src/game/CTurnManager.h \
  src/game/CScene.h \
  src/game/CRenderer.h \
  src/game/CPexesoScene.h \
  src/gui/CPexesoView.h \
  src/gui/CGameMenuWidget.h \
  src/gui/CGameOverWidget.h \
  src/gui/CImageWidget.h \
  src/gui/CCreditsLabel.h \
  src/gui/CSelectionBox.h \
  src/gui/CSceneBrowser.h \
  src/gui/CStyledWidget.h \
  src/gui/CMainWindow.h \
  src/gui/CMainMenuWidget.h \
  src/gui/CSinglePlayerMenuWidget.h \
  src/gui/CMultiPlayerMenuWidget.h \
  src/Global.h \
    src/game/CFileManager.h \
    src/game/CGameStats.h

# Source files
SOURCES += \
  src/maths/Maths.cpp \
  src/ai/CBaseAiAgent.cpp \
  src/game/ogl.cpp \
  src/game/CTextureStore.cpp \
  src/game/CIndexBuffer.cpp \
  src/game/CVertexBuffer.cpp \
  src/game/CBBox.cpp \
  src/game/CBaseModel.cpp \
  src/game/CCubeModel.cpp \
  src/game/CTrackBall.cpp \
  src/game/CTurnManager.cpp \
  src/game/CScene.cpp \
  src/game/CRenderer.cpp \
  src/game/CPexesoScene.cpp \
  src/gui/CGameMenuWidget.cpp \
  src/gui/CGameOverWidget.cpp \
  src/gui/CPexesoView.cpp \
  src/gui/CSelectionBox.cpp \
  src/gui/CSceneBrowser.cpp \
  src/gui/CStyledWidget.cpp \
  src/gui/CMainWindow.cpp \
  src/gui/CMainMenuWidget.cpp \
  src/gui/CSinglePlayerMenuWidget.cpp \
  src/gui/CMultiPlayerMenuWidget.cpp \
  src/main.cpp \
    src/game/CFileManager.cpp \
    src/game/CGameStats.cpp

# Resource files
RESOURCES += \
  pexeso.qrc
