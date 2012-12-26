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
 * This file implements the model base class
 */

#include <QDebug>

#include "CBaseModel.h"


/**
 */
QDebug & operator<<(QDebug & debug, const CBaseModel & model)
{
  debug.nospace() << "CBaseModel("
                  << model.m_bbox
                  << ", "
                  << model.m_id
                  << ", "
                  << model.m_tex_ind
                  << ", "
                  << ((model.m_state.covered) ? ("covered") : ("NOT covered"))
                  << ", "
                  << ((model.m_state.selected) ? ("selected") : ("NOT selected"))
                  << ", "
                  << ((model.m_state.deleted) ? ("deleted") : ("NOT deleted"))
                  << ")";

  return debug.maybeSpace();
}
