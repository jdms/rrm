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
 * @file dipanglepicture.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class DipAnglePicture
 */


#include "dipanglepicture.h"

DipAnglePicture::DipAnglePicture()
{
    setAcceptedMouseButtons( Qt::LeftButton|Qt::RightButton ) ;

    setFlag( QGraphicsItem::ItemIgnoresTransformations, true );
}

void DipAnglePicture::setImage( const QPixmap &im )
{
    setPixmap( im );
    update();
}

void DipAnglePicture::setMovable( bool status_ )
{
    setFlag( QGraphicsItem::ItemIsSelectable, status_ );
    setFlag( QGraphicsItem::ItemIsMovable, status_ );
    update();
}
