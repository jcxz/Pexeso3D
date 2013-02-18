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
 * This file defines some globaly useful macros and constants
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtGlobal>
#include <cassert>

/** Silence MSVC CRT warnings */
#if defined(Q_OS_WIN32)
# define _CRT_SECURE_NO_WARNINGS
#endif

/** An assertion macro */
#define PEXESO_ASSERT assert

/** Define portable line, function, and file macros */
# define PEXESO_FILE __FILE__
# define PEXESO_LINE __LINE__
#if defined(Q_CC_MSVC)
# define PEXESO_FUNC __FUNCSIG__
#elif defined(Q_CC_GCC)
# define PEXESO_FUNC __PRETTY_FUNCTION__
#else
# define PEXESO_FUNC __FUNCTION__
#endif

/** A macro to designate unused parameters */
#define PEXESO_UNUSED(x) ((void) (x))

/**
 * Disable copying of c++ classes and structures
 * This piece of code was inspired by qt sources,
 * have a look at qglobal.h to find out more.
 * @Note you need to use this macro after private/protected
 * access modifier for it to work properly
 */
#define PEXESO_DISABLE_COPY(cls_name) \
  cls_name(const cls_name & ); \
  cls_name & operator=(const cls_name & )

/**
 * A macro to clamp the value to a given range
 */
#define PEXESO_CLAMP(val, min, max) \
  (((val) < (min)) ? \
    (min) : \
    (((val) > (max)) ? \
      (max) : \
      (val) \
    ) \
  )

/**
 * This macro will return an offset of member variable
 */
#define PEXESO_OFFSETOF(cls, m) (&(static_cast<cls *>(0)->m))

/**
 * This macro will return an offset of member variable
 * casted to (const char *)
 */
#define PEXESO_MEMBER_OFFSET(cls, m) ((const char *) PEXESO_OFFSETOF(cls, m))

/**
 * A constant defining application name
 */
/* note in C++ const means implicitly an internal linkage,
   but to declare constant strings, both pointer and the string
   have to be constant, otherwise it will cause linker problems */
const char * const g_app_name = "Pexeso";

/**
 * A credits message
 */
const char * const g_credits = "Created by Matus Fedorko, Peter Pilat, Josef Rudolf\n"
                               "For more information contact us at: 3dpexeso@gmail.com";

/**
 * A default directory for scene files
 */
const char * const g_scene_dir = "data";

/**
 * A file that contains the list of scenes that can be loaded
 */
const char * const g_scene_list_file = "data/scenes/scenes.xml";

/**
 * A default directory for stylesheets
 */
const char * const g_qss_dir = "gui";

/**
 * A default stylesheet
 */
const char * const g_qss_file = "gui/style.qss";

#endif
