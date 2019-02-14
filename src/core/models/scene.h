/****************************************************************************
 * RRM - Rapid Reservoir Modeling Project                                   *
 * Copyright (C) 2015                                                       *
 * UofC - University of Calgary                                             *
 *                                                                          *
 * This file is part of RRM Software.                                       *
 *                                                                          *
 * RRM is free software: you can redistribute it and/or modify              *
 * it under the terms of the GNU General Public License as published by     *
 * the Free Software Foundation, either version 3 of the License, or        *
 * (at your option) any later version.                                      *
 *                                                                          *
 * RRM is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 * GNU General Public License for more details.                             *
 *                                                                          *
 * You should have received a copy of the GNU General Public License        *
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.             *
 ****************************************************************************/

/**
 * @file scene.h
 * @author Clarissa C. Marques
 * @brief File containing the class Scene
 */

#ifndef SCENE_H
#define SCENE_H


#include "./core/definitions/constants.hpp"

#include "volume.h"
#include "crosssection.h"
#include "object.h"
#include "regions.h"

/**
 *  A representation to the scene
 */

class Scene
{

    public:


        /**
        * Constructor.
        */
        Scene() = default;


        /**
        * A pure virtual method to create a volume in the scene.
        * This volume will hold a pointer to a volume data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @param raw data structure that contains the geometry and others required information to draw it on scene.
        * @see Volume
        * @return Void.
        */
        virtual void addVolume( Volume* const& raw_ ) = 0;


        /**
        * A pure virtual method to remove the volume in the scene.
        * @see Volume
        * @return Void.
        */
        virtual void clearVolume() = 0;


        /**
        * A pure virtual method to update the volume in the scene.
        * @see Volume
        * @return Void.
        */
        virtual void updateVolume() = 0;


        /**
        * A pure virtual method to add stratigraphies and structurals in the scene.
        * This stratigraphy will hold a pointer to a stratigraphy data structure, so that it can retrieve the required information
        * to draw it on scene. Note that currently a structural has the same data structure as the stratigraphy.
        * @param raw data structure that contains the geometry and others required information to draw it on scene.
        * @return Void.
        */
        virtual void addObject(  Object* const& raw_ ) = 0;


        /**
        * A pure virtual method to update the stratigraphy or structural, which index is id_.
        * This update can be since updating the dimensions, color to its visibility.
        * @param id index of the stratigraphy to be updated.
        * @see Object
        * @return Void.
        */
        virtual void updateObject(  const std::size_t& index_ ) = 0;
//        virtual void selectObject();


        /**
        * A pure virtual method to add cross-section in the scene.
        * This cross-section will hold a pointer to a cross-section data structure, so that it can retrieve the required information
        * to draw it on scene.
        * @param raw data structure that contains the geometry and others required information to draw it on scene.
        * @see CrossSection
        * @return Void.
        */
        virtual void addCrossSection( CrossSection* const& raw_ ) = 0;


        /**
        * A pure virtual method to remove a cross-section in the scene.
        * @param raw data structure that contains the geometry and others required information to draw it on scene.
        * @see CrossSection
        * @return Void.
        */
        virtual void removeCrossSection( CrossSection* const& raw_ ) = 0 ;
//        virtual void updateCrossSection( CrossSection* const& raw_ ) = 0;
//        virtual void updateCrossSection();


//        virtual void addRegion() = 0;
//        virtual void updateRegion() = 0;


        /**
        * A pure virtual method to update the current color in the scene
        * @param r the red component of the color (integer)
        * @param g the green component of the color (integer)
        * @param b the blue component of the color (integer)
        * @return Void
        */
        virtual void setCurrentColor( int r, int g, int b ) = 0;


        /**
        * A pure virtual method to get the current color in the scene
        * @param r a reference to the red component of the color (integer)
        * @param g a reference to the green component of the color (integer)
        * @param b a reference to the blue component of the color (integer)
        * @return Void
        */
        virtual void getCurrentColor( int& r, int& g, int& b ) = 0;



};

#endif // SCENE_H
