/** @license
 * RRM - Rapid Reservoir Modeling Project
 * Copyright (C) 2015
 * UofC - University of Calgary
 *
 * This file is part of RRM Software.
 *
 * RRM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RRM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with RRM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "BoundaryItem2D.hpp"

#include <iostream>

BoundaryItem2D::BoundaryItem2D()
{

    prepareGeometryChange();

}

BoundaryItem2D::~BoundaryItem2D()
{
}



void BoundaryItem2D::clear()
{
    boundary.setCoords( 0, 0, 0, 0 );
    image_position = QPointF( 0.0f, 0.0f );
}



void BoundaryItem2D::edit( const qreal& x, const qreal& y, const qreal& width, const qreal& height )
{


    prepareGeometryChange();
    boundary.setRect( x, y, width, height );

}


void BoundaryItem2D::paint( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{

    painter->setRenderHint ( QPainter::Antialiasing );


    QPen pen_color ( QColor ( 0 , 200 , 0 ) );
    pen_color.setWidth ( 1 );

    QBrush brush;
    brush.setColor( QColor( 55, 100, 55, 75 ) );
    brush.setStyle( Qt::SolidPattern );


    painter->setBrush( brush );
    painter->setPen ( pen_color );
    painter->drawRect( boundary );


}


QRectF BoundaryItem2D::boundingRect() const
{
    return boundary;
}


void BoundaryItem2D::update( const Eigen::Affine3f& m )
{

    prepareGeometryChange();


    Eigen::Vector4f A( bd->getX(),                   bd->getY(),                   0.0f, 1.0f );
    Eigen::Vector4f B( bd->getX() + bd->getWidth(),  bd->getY() + bd->getHeight(), 0.0f, 1.0f );

    A = m*A;
    B = m*B;

    int width  = ( int )( B.x() - A.x() );
    int height = ( int )( B.y() - A.y() );

    boundary.setRect( A.x(), A.y(), width, height );


}

