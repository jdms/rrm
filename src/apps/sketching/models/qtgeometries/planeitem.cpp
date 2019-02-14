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
 * @file planeitem.cpp
 * @author Clarissa C. Marques
 * @date 9 Sep 2012
 * @brief File containing the class PlaneItem
 */


#include <QPainter>

#include "planeitem.h"

PlaneItem::PlaneItem( QGraphicsItem *parent_ ): QGraphicsRectItem( parent_ )
{
    setRect( plane );
}

QRectF PlaneItem::boundingRect() const
{
    // test
    QPainterPath path_;
    path_.addRect( plane );
    return path_.boundingRect();
//    return plane;
}


void PlaneItem::paint( QPainter * painter_, const QStyleOptionGraphicsItem * option_, QWidget * widget_ )
{
    if( isVisible() == false )
    {
        return;
    }


    QPen border_;
    border_.setColor( getBorderColor() );
    border_.setWidth( getBorderWidth() );
    border_.setCosmetic( true );

    QBrush fill_;
    QColor color_ = getFillColor();
    fill_.setColor(  QColor( color_.red(), color_.green(), color_.blue(), 75 ) );
    fill_.setStyle( Qt::SolidPattern );

    if( isSelected() == true )
    {
        border_.setColor( Qt::blue );
    }

    painter_->setRenderHint( QPainter::Antialiasing );
    painter_->setPen( border_ );
    painter_->setBrush( fill_ );
    painter_->drawRect( plane );
}


void PlaneItem::setStartPoint( const QPointF& start_ )
{
    prepareGeometryChange();
    plane.setTopLeft( start_ );
    update();
}

void PlaneItem::setEndPoint( const QPointF& end_ )
{
    prepareGeometryChange();
    plane.setBottomRight( end_ );
    update();
}


void PlaneItem::resize( const QPointF& dimensions_, const QPointF& origin_ )
{
    prepareGeometryChange();
    plane = QRectF( origin_, dimensions_ );
    update();
}


void PlaneItem::resize( double width_, double height_, double ox_, double oy_ )
{
    resize( QPointF( width_, height_), QPointF( ox_, oy_ ) );
}

void PlaneItem::setBorderColor( int r_, int g_, int b_ )
{
    setBorderColor( QColor( r_, g_, b_ ) );
}

void PlaneItem::getBorderColor( int& r_, int& g_, int& b_ ) const
{
    r_ = border_color.red  ;
    g_ = border_color.green;
    b_ = border_color.blue ;
}

void PlaneItem::setBorderColor( const QColor& color_ )
{
    prepareGeometryChange();
    border_color.red   = color_.red();
    border_color.green = color_.green();
    border_color.blue  = color_.blue();
    update();

}

QColor PlaneItem::getBorderColor() const
{
    return QColor( border_color.red, border_color.green, border_color.blue );
}

void PlaneItem::setBorderWidth( int width_ )
{
    prepareGeometryChange();
    border_width = width_;
    update();
}

int PlaneItem::getBorderWidth() const
{
    return border_width;
}

void PlaneItem::setFillColor( int r_, int g_, int b_ )
{
    setFillColor( QColor( r_, g_, b_ ) );
}

void PlaneItem::getFillColor( int& r_, int& g_, int& b_ ) const
{
    r_ = fill_color.red  ;
    g_ = fill_color.green;
    b_ = fill_color.blue ;
}

void PlaneItem::setFillColor( const QColor& color_ )
{
    prepareGeometryChange();

    fill_color.red   = color_.red();
    fill_color.green = color_.green();
    fill_color.blue  = color_.blue();
    update();

}

QColor PlaneItem::getFillColor() const
{
    return QColor( fill_color.red, fill_color.green, fill_color.blue );
}


void PlaneItem::clear()
{
    prepareGeometryChange();

    plane = QRectF();
    setRect( plane );

    resetToDefaultBorderColor();
    resetToDefaultFillColor();
    resetToDefaultBorderWidth();

    update();
}


bool PlaneItem::isEmpty() const
{
    return plane.isEmpty();
}

void PlaneItem::resetToDefaultBorderColor()
{
    setBorderColor( DEFAULT_BORDER_COLOR.red, DEFAULT_BORDER_COLOR.green, DEFAULT_BORDER_COLOR.blue );
}

void PlaneItem::resetToDefaultFillColor()
{
    setFillColor( DEFAULT_FILL_COLOR.red, DEFAULT_FILL_COLOR.green, DEFAULT_FILL_COLOR.blue );
}

void PlaneItem::resetToDefaultBorderWidth()
{
    setBorderWidth( DEFAULT_BORDER_WIDTH );
}


PlaneItem::~PlaneItem()
{
    clear();
}
