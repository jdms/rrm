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
 * @file objecttreeitem.h
 * @author Clarissa C. Marques
 * @brief File containing the class ObjectTreeItem
 */


#ifndef OBJECTTREEITEM_H
#define OBJECTTREEITEM_H


#include <QTreeWidgetItem>

#include "./core/definitions/constants.hpp"


/**
 *  An extension of QTreeWidgetItem to represent the objects presents in the RRM model scene.
 */

class ObjectTreeItem: public QTreeWidgetItem
{
    public:

        /**
        * Constructor.
        */
        ObjectTreeItem();


        /**
        * Method to set the type of the object
        * @param type the type of the object, i.e., it can be 'NONE, 'VOLUME', 'CROSS_SECTION',
        * 'STRATIGRAPHY', 'STRUCTURAL', 'REGION', or 'DOMAINS'
        * @see Settings::Objects::ObjectType
        * @return void.
        */
        void setType( const Settings::Objects::ObjectType& type_ );


        /**
        * Method to return the type of the object
        * @param type the type of the object
        * @see Settings::Objects::ObjectType
        * @return ObjectType the type of the object, i.e., it can be 'NONE, 'VOLUME', 'CROSS_SECTION',
        * 'STRATIGRAPHY', 'STRUCTURAL', 'REGION', or 'DOMAINS'
        */
        Settings::Objects::ObjectType getType() const;


        /**
        * Method to set the index of the object
        * @param index index of the object
        * @return void.
        */
        void setIndex( const std::size_t index_ );


        /**
        * Method to return the index of the object
        * @return size_t index of the object
        */
        std::size_t getIndex() const;


    protected:


        std::size_t index;                                      /**< Index of the object */

        Settings::Objects::ObjectType type;                     /**< Type of the object, it can be 'NONE, 'VOLUME', 'CROSS_SECTION',
        * 'STRATIGRAPHY', 'STRUCTURAL', 'REGION', or 'DOMAINS' */


};

#endif // OBJECTTREEITEM_H
