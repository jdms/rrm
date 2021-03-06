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
 * @file stratigraphyitem.h
 * @author Clarissa C. Marques
 * @brief File containing the class StratigraphyItem
 */


#ifndef STRATIGRAPHYITEM_H
#define STRATIGRAPHYITEM_H

#include "./core/models/stratigraphy.h"
#include "./core/models/crosssection.h"
#include "./qtgeometries/curveitem.h"


/**
 *  A graphical representation to the stratigraphy and structural in the scene 2d.
 *  It is represent by a curve since it is the intersection between a surface with the cross-section plane.
 *  It inherits from CurveItem.
 */

class StratigraphyItem: public CurveItem
{
    public:


        /**
        * Constructor. parent is passed to QGraphicsLineItem's constructor.
        * @param parent the parent item
        */
        StratigraphyItem( QGraphicsItem *parent_ = nullptr );


        /**
        * Destructor.
        */
        ~StratigraphyItem() override;


        /**
        * Method to set a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved.
        * @param raw a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        * @param dir the direction of the cross-section that cutted the surface
        * @param depth the depth of the cross-section that cutted the surface
        * @see Stratigraphy
        * @see Settings::CrossSection::CrossSectionDirections
        * @return void.
        */
        void setRawStratigraphy( const std::shared_ptr< Stratigraphy >& raw_, const Settings::CrossSection::CrossSectionDirections& dir_, double depth_  );


        /**
        * Method to return the const shared point to the data structure of the stratigraphy
        * @see Stratigraphy
        * @return a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        */
        const std::shared_ptr< Stratigraphy >& getRawStratigraphy() const;


        /**
        * Method to set the cross-section which cutted the surface
        * @param dir the direction of the cross-section
        * @param depth the depth of the cross-section
        * @see Settings::CrossSection::CrossSectionDirections
        * @return a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        */
        void setCrossSection( const Settings::CrossSection::CrossSectionDirections& dir_, double depth_ );


        /**
        * Method to return the index of the stratigraphy/structural
        * @return a std::size_t representing the index of the stratigraphy/structural
        */
        std::size_t getIndex() const;


        /**
        * This method checks if the stratigraphy/structural should be visible or not
        * @return boolean. Returns true if the stratigraphy/structural is visible, and false otherwise.
        */
        bool isVisible() const override;


        /**
        * This method checks if the stratigraphy/structural is selected or not
        * @return boolean. Returns true if the stratigraphy/structural is selected, and false otherwise.
        */
        bool isSelected() const override;


        /**
        * This method calls the method 'updateCurve'
        * @return void.
        */
        void update() override;


        /**
        * This method updates only the trajectory of the stratigraphy/structural if it exists
        * @return void.
        */
        void updateTrajectory();


        /**
        * This method updates only the level curves  of the stratigraphy/structural if the curve was made in the height direction
        * @return void.
        */
        void updateLevelCurves();


        /**
        * This method reset the shared pointer to the stratigraphy/structural data structure and clear the cross-section information
        * @return void.
        */
        void clear();


    protected:


        /**
        * This method updates the geometry and others rendering properties
        * @return void.
        */
        void updateCurve();


    protected:

        std::shared_ptr< Stratigraphy > raw;                                                    /**< Shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved. */

        Settings::CrossSection::CrossSectionDirections csection_direction =                     /**< Direction of the cross-section that cutted the surface */
                Settings::CrossSection::DEFAULT_CSECTION_DIRECTION;

        double csection_depth = 0.0;                                                            /**< Depth of the cross-section that cutted the surface */

};

#endif // STRATIGRAPHYITEM_H
