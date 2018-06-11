/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */



#ifndef SCENE_H
#define SCENE_H


#include "./core/definitions/constants.hpp"

#include "volume.h"
#include "crosssection.h"
#include "object.h"
#include "regions.h"


class Scene
{

    public:

        Scene() = default;


        virtual void addVolume( Volume* const& raw_ ) = 0;
        virtual void clearVolume() = 0;
        virtual void updateVolume() = 0;


        virtual void addObject(  Object* const& raw_ ) = 0;
        virtual void updateObject(  const std::size_t& index_ ) = 0;
//        virtual void selectObject();


        virtual void addCrossSection( CrossSection* const& raw_ ) = 0;
        virtual void removeCrossSection( CrossSection* const& raw_ ) = 0 ;
//        virtual void updateCrossSection( CrossSection* const& raw_ ) = 0;
//        virtual void updateCrossSection();


//        virtual void addRegion() = 0;
//        virtual void updateRegion() = 0;


        virtual void setCurrentColor( int r, int g, int b ) = 0;
        virtual void getCurrentColor( int& r, int& g, int& b ) = 0;



};

#endif // SCENE_H
