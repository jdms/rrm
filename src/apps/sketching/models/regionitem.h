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
 * @file regionitem.h
 * @author Clarissa C. Marques
 * @brief File containing the class RegionItem
 */


#ifndef REGIONITEM_H
#define REGIONITEM_H

#include <memory>

#include "./core/models/regions.h"
#include "./qtgeometries/polygonitem.h"
#include "./qtgeometries/curveitem.h"

/**
 *  A graphical representation to the region in the scene 2d.
 *  It is represent by a polygon since it is the intersection between a volume with the cross-section plane.
 *  It inherits from PolygonItem.
 */

class RegionItem: public PolygonItem
{
    public:

        /**
        * Constructor. parent is passed to PolygonItem's constructor.
        * @param parent the parent item
        */
        RegionItem( QGraphicsItem *parent_ = nullptr );


        /**
        * Destructor.
        */
        ~RegionItem() override;


        /**
        * Method to set a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved.
        * @param raw a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        * @param dir the direction of the cross-section that cutted the volume
        * @param depth the depth of the cross-section that cutted the volume
        * @see Regions
        * @return Void.
        */
        void setRawRegion( const std::shared_ptr< Regions >& raw_ );


        /**
        * Method to return the const shared point to the data structure of the stratigraphy
        * @see Regions
        * @return a const shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved
        */
        const std::shared_ptr< Regions >& getRawRegion() const;


        /**
        * Method to return the index of the stratigraphy/structural
        * @return a std::size_t representing the index of the stratigraphy/structural
        */
        std::size_t getIndex() const;


        /**
        * This method checks if the region should be visible or not
        * @return boolean. Returns true if the region is visible, and false otherwise.
        */
        virtual bool isVisible() const override;


        /**
        * This method checks if the region is selected or not
        * @return boolean. Returns true if the region is selected, and false otherwise.
        */
        virtual bool isSelected() const override;


        /**
        * This method updates the boundary of the polygon as also rendering properties.
        * @return boolean. Returns true if the region is selected, and false otherwise.
        */
        void updateBoundary();


        /**
        * This method reset the shared pointer to the region data structure and clear the cross-section information
        * @return Void
        */
        void clear();


    private:


        std::shared_ptr< Regions > raw;                                         /**< Shared point to the data structure, from where the geometry, visibility and others relevants information to the rendering should be retrieved. */

};

#endif // REGIONITEM_H
