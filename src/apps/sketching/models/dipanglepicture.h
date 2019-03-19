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
 * @file dipanglepicture.h
 * @author Clarissa C. Marques
 * @brief File containing the class DipAnglePicture
 */



#ifndef DIPANGLEPICTURE_H
#define DIPANGLEPICTURE_H


#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QString>

/**
 *  Auxiliary wrapper to hold and handle operations on the output dip angle image.
 *  It inherits from QGraphicsPixmapItem.
 */


class DipAnglePicture: public QGraphicsPixmapItem
{
    public:

        /**
        * Constructor.
        */
        DipAnglePicture();


        /**
        * Method to set the image
        * @param im the image
        * @see QPixmap
        * @return void.
        */
        void setImage( const QPixmap& im );


        /**
        * Method to set the image movable if the image should be movable or not.
        * @param status boolean to indicate if the image should be movable or not.
        * It is true if the image is movable and false otherwise.
        * @return void.
        */
        void setMovable( bool status_ );

};

#endif // DIPANGLEPICTURE_H
