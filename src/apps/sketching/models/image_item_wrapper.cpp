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
 * @file image_item_wrapper.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class ImageItemWrapper
 */


#include <iostream>

#include "image_item_wrapper.h"



ImageItemWrapper::ImageItemWrapper()
{
    setAcceptedMouseButtons( Qt::LeftButton|Qt::RightButton ) ;
    update();

}


QRectF ImageItemWrapper::boundingRect() const
{
    return QGraphicsPixmapItem::boundingRect();
}




void ImageItemWrapper::paint( QPainter* painter, const QStyleOptionGraphicsItem* option,
                QWidget* widget )
{
    // fill the rectangle m_imageRect with the image
    painter->drawPixmap( m_imageRect.toRect(), m_image );
}


void ImageItemWrapper::resizeRectangle( const QPointF& p )
{
    m_imageRect.setBottomRight( p );
    update();
}


void ImageItemWrapper::moveRectangle( QPointF origin_ )
{
    m_imageRect.moveTo( origin_.x(), origin_.y() );
    update();

}


void ImageItemWrapper::setImage( const QPixmap &im, QPointF origin_, QPointF top_right_ )
{
    m_image = im;
    moveRectangle( origin_ );
    resizeRectangle( top_right_ );
    setPixmap( m_image );
    update();

}



void ImageItemWrapper::setImagePath( const QString& file_ )
{
    path = file_;
}


QString ImageItemWrapper::getImagePath() const
{
    return path;
}

\
QPointF ImageItemWrapper::getOrigin() const
{
    return m_imageRect.topLeft();
}


QPointF ImageItemWrapper::getTopRight() const
{
    return m_imageRect.bottomRight();
}

