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
 * @file volumeitem.h
 * @author Clarissa C. Marques
 * @brief File containing the class VolumeItem
 */


#ifndef VOLUMEITEM_H
#define VOLUMEITEM_H

#include "./core/models/volume.h"
#include "./core/models/crosssection.h"
#include "./qtgeometries/planeitem.h"


/**
 *  A graphical representation to the volume in the scene 2d.
 *  It always is represent by a plane since it is a slice of the volume in a direction and depth.
 *  It inherits from PlaneItem
 */

class VolumeItem: public PlaneItem
{
    public:

        /**
        * Constructor. parent is passed to PlaneItem's constructor.
        * @param parent the parent item
        */
        VolumeItem( QGraphicsItem *parent_ = nullptr );

        /**
        * Destructor.
        */
        ~VolumeItem() override;

        /**
        * Method to set a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved.
        * @param raw a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        * @param dir the direction of the cross-section
        * @param depth the depth of the cross-section
        * @see Volume
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void setRawVolume( const std::shared_ptr< Volume >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method to return the const shared point to the data structure of the volume
        * @see Volume
        * @return a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        */
        const std::shared_ptr< Volume >& getRawVolume() const;


        /**
        * Method to update the dimensions given a new direction.
        * @param dir a new direction
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void updateDimensions( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * This method calls the method 'updateDimensions'.
        * @param dir a new direction
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void update( const Settings::CrossSection::CrossSectionDirections& dir_ );


        /**
        * This method checks if the volume should be visible or not
        * @return boolean. Returns true if the volume is visible, and false otherwise.
        */
        virtual bool isVisible() const override ;


        /**
        * This method clears all the data of the class, including to reset the shared pointer of the Volume data structure.
        * @return void.
        */
        void clear();

    private:

        std::shared_ptr< Volume > raw;                                          /**< Shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved. */

        Settings::CrossSection::CrossSectionDirections csection_direction =     /**< Direction of the slice of the volume */
                Settings::CrossSection::DEFAULT_CSECTION_DIRECTION;

        double csection_depth = 0.0;                                            /**< Depth of the slice of the volume in the set direction */

};

#endif // VOLUMEITEM_H
