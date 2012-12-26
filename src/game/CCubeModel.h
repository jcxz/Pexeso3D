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
 * This file defines a game model of a cube
 */

#ifndef CCUBEMODEL_H
#define CCUBEMODEL_H

#include "CBaseModel.h"


/**
 * This class defines a cubic model
 */
class CCubeModel : public CBaseModel
{
  public:
    /**
     * Constructor
     */
    CCubeModel(void)
      : CBaseModel()
    {
    }

    /**
     * This method will build the cube model at a given
     * positon with the given side
     *
     * @param pos of the model in world space
     * @param side the size of the cube model
     *
     * @return true if building the model was successfull,
     *         false otherwise
     */
    bool build(const Maths::SVector3D & pos, GLfloat side);
};

#endif
