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
 * @file polygonitem.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class PolygonItem
 */

#include <iostream>

#include <QPainter>

#include "polygonitem.h"


PolygonItem::PolygonItem( QGraphicsItem *parent_ ): QGraphicsPolygonItem( parent_ )
{
//    setPolygon( plane );
}


QRectF PolygonItem::boundingRect() const
{
    return plane.boundingRect();
}


QPainterPath PolygonItem::shape() const
{
    QPainterPath path;
    path.addPolygon(plane);
    return path;
}

void PolygonItem::setPolygon( const QPolygonF& pol_ )
{
    prepareGeometryChange();
    plane.clear();
    plane = pol_;
    update();
}

void PolygonItem::paint( QPainter * painter_, const QStyleOptionGraphicsItem * option_, QWidget * widget_ )
{
    if( isVisible() == false )
        return;


    QPen border_;
    border_.setColor( getBorderColor() );
    border_.setWidth( getBorderWidth() );
    border_.setCosmetic( true );

    QBrush fill_;
    QColor color_ = getFillColor();

    if( is_transparent == true )
        fill_.setColor(  QColor( color_.red(), color_.green(), color_.blue(), 75 ) );
    else
        fill_.setColor(  QColor( color_.red(), color_.green(), color_.blue() ) );

    fill_.setStyle( Qt::SolidPattern );


    painter_->setRenderHint( QPainter::Antialiasing );

    if( has_border == true )
        painter_->setPen( border_ );
    else
        painter_->setPen( Qt::NoPen );

    if( QGraphicsPolygonItem::isSelected() == true )
    {
        // change the style to fill it with a pattern if it is selected
        fill_.setStyle( Qt::Dense3Pattern);
    }

    painter_->setBrush( fill_ );
    painter_->drawPolygon( plane );
}



void  PolygonItem::addPoint( const QPointF& p_ )
{
    prepareGeometryChange();
    plane.push_back( p_ );
    update();
}

void  PolygonItem::removePoint( const QPointF& p_ )
{

    int index_ = plane.indexOf( p_ );
    if( index_ < 0 ) return;

    prepareGeometryChange();
    plane.remove( index_ );
    update();
}

void  PolygonItem::setDone( bool status_ )
{
    is_done = status_;
}

bool  PolygonItem::isDone() const
{
    return is_done;
}

void PolygonItem::setBorderColor( int r_, int g_, int b_ )
{
    setBorderColor( QColor( r_, g_, b_ ) );
    update();
}

void PolygonItem::getBorderColor( int& r_, int& g_, int& b_ ) const
{
    r_ = border_color.red  ;
    g_ = border_color.green;
    b_ = border_color.blue ;
}

void PolygonItem::setBorderColor( const QColor& color_ )
{
    prepareGeometryChange();
    border_color.red   = color_.red();
    border_color.green = color_.green();
    border_color.blue  = color_.blue();
    update();

}

QColor PolygonItem::getBorderColor() const
{
    return QColor( border_color.red, border_color.green, border_color.blue );
}

void PolygonItem::setBorderWidth( int width_ )
{
    prepareGeometryChange();
    border_width = width_;
   update();
}

int PolygonItem::getBorderWidth() const
{
    return border_width;
}


void PolygonItem::setBorderVisible( bool status_ )
{
    has_border = status_;
    update();
}

bool PolygonItem::isBorderVisible() const
{
    return has_border;
}


void PolygonItem::setTransparency( bool status_ )
{
    is_transparent = status_;
    update();
}


bool PolygonItem::hasTransparency() const
{
    return is_transparent;
}

void PolygonItem::setFillColor( int r_, int g_, int b_ )
{
    setFillColor( QColor( r_, g_, b_ ) );
    update();

}

void PolygonItem::getFillColor( int& r_, int& g_, int& b_ ) const
{
    r_ = fill_color.red  ;
    g_ = fill_color.green;
    b_ = fill_color.blue ;
}

void PolygonItem::setFillColor( const QColor& color_ )
{
    prepareGeometryChange();
    fill_color.red   = color_.red();
    fill_color.green = color_.green();
    fill_color.blue  = color_.blue();
    update();

}

QColor PolygonItem::getFillColor() const
{
    return QColor( fill_color.red, fill_color.green, fill_color.blue );
}


void PolygonItem::clear()
{
    prepareGeometryChange();
    plane.clear();

    update();
}


bool PolygonItem::isEmpty() const
{
    return plane.isEmpty();
}

void PolygonItem::resetToDefaultBorderColor()
{
    setBorderColor( DEFAULT_BORDER_COLOR.red, DEFAULT_BORDER_COLOR.green, DEFAULT_BORDER_COLOR.blue );
}

void PolygonItem::resetToDefaultFillColor()
{
    setFillColor( DEFAULT_FILL_COLOR.red, DEFAULT_FILL_COLOR.green, DEFAULT_FILL_COLOR.blue );
}

void PolygonItem::resetToDefaultBorderWidth()
{
    setBorderWidth( DEFAULT_BORDER_WIDTH );
}


PolygonItem::~PolygonItem()
{
    clear();
}


