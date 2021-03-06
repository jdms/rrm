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
 * @file coordinateaxes_2d.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class CoordinateAxes2d
 */

#include <cmath>
#include <iostream>

#include <QPen>
#include <QGraphicsScene>

#include "coordinateaxes_2d.h"

#define PI 3.14159265

CoordinateAxes2d::CoordinateAxes2d( const Plane& pl )
{
    setPlane( pl );
    axisy_length = 500;
    axisx_length = 500;
}


void CoordinateAxes2d::setPlane( const Plane& pl )
{

    plane = pl;

    // the scale invert the axes orientation
    if( pl == Plane::XY )
    {
        current_y = height_color;
        current_x = width_color;
        scale = -1;
    }
    else if( pl == Plane::YZ )
    {
        current_x = depth_color;
        current_y = height_color;
        scale = -1;
    }
    else
    {
        scale = 1;
        current_x = width_color;
        current_y = depth_color;
    }
}

void CoordinateAxes2d::setAxisXLenght( double l )
{
    axisx_length = l;
    update();
}

void CoordinateAxes2d::setAxisYLenght( double l )
{
    axisy_length = l;
    update();
}


QRectF CoordinateAxes2d::boundingRect() const
{
    return QRectF( -20, 20, axisx_length + 20, axisy_length + 20 );
}


void CoordinateAxes2d::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    drawAxisX( painter );

    // if scale_on is false so no vertical exaggeration is applied
    if( scale_on == false )
        drawAxisY( painter );
    else
        drawAxisYWithScale( painter );
}


void CoordinateAxes2d::drawAxisX( QPainter* p )
{
    // code needs to be improve. temporary code


    QPointF tail = QPointF( -15, -15 );

    // initial discretization of the X axes.
    int discX = 10;

    // it changes depends on the lenght of the axes
    if( axisx_length < 150 )
        discX = 1;

    else if( axisx_length < 300 )
        discX = 3;

    else if( axisx_length < 500 )
        discX = 7;

    else if( axisx_length > 700 )
        discX += static_cast< int >(discX*0.7 );

    double stepX = static_cast< double >( axisx_length/ discX );


    QPen pen;
    pen.setWidth( 1 );
    pen.setColor( current_x );

    p->setPen( pen );
    p->drawLine( tail.x(), tail.y(), axisx_length, tail.y() );


    for( int i = 0; i < discX + 1; ++i )
    {
        double value = i*stepX;
        double dez = 0;

        if( value > 0 )
            dez = log10( value );


        QString value1_ = QString::number( value, 'f', 1);

        // render the discretization lines
        p->drawLine( value , tail.y() - 3, value , tail.y() + 3 );
        p->scale( 1, scale );

        // render the texts which spaces of each other depends on the logarithm natural
        p->drawText( QPointF( value - dez*0.1*stepX, scale*tail.y() - scale*18 ), QString( "%1" ).arg( value1_ ) );
        p->scale( 1, scale );
    }


}

void CoordinateAxes2d::drawAxisY( QPainter* p )
{

    // rendering the Y axes without vertical exaggeration
    QPointF tail = QPointF( -15, -15 );

    int discY = 10;

    if( axisy_length < 150 )
        discY = 1;

    else if( axisy_length < 300 )
        discY = 3;

    else if( axisy_length < 500 )
        discY = 7;

    else if( axisy_length > 700 )
        discY += static_cast< int >(discY*0.7 );


    double stepY = static_cast< double >( axisy_length/ discY );


    QPen pen;
    pen.setWidth( 1 );
    pen.setColor( current_y );
    p->setPen( pen );
    p->drawLine( tail.x(), tail.y(), tail.x(), axisy_length );

    for( int i = 0; i < discY + 1; ++i )
    {
        double value = i*stepY;
        double dez = 0;

        if( value > 0 )
            dez = log10( value );
        p->drawLine( tail.x() - 3 , value, tail.x() + 3 , value );

        QString value1_ = QString::number( value, 'f', 1);

        p->rotate( -scale*90 );
        p->scale( 1, scale );
        p->drawText( QPointF( -scale*value - dez*0.1*stepY, tail.x() - 10 ), QString( "%1" ).arg( value1_ ) );
        p->scale( 1, scale );
        p->rotate( scale*90 );
    }

}



void CoordinateAxes2d::drawAxisYWithScale( QPainter* p )
{

    // rendering the Y axes with vertical exaggeration
    QPointF tail = QPointF( -15, -15 );

    int discY = 10;

    if( axisy_length < 150 )
        discY = 1;

    else if( axisy_length < 300 )
        discY = 3;

    else if( axisy_length < 500 )
        discY = 7;

    else if( axisy_length > 700 )
        discY += static_cast< int >(discY*0.7 );

    double stepY = static_cast< double >( axisy_length/ discY );


    QPen pen;
    pen.setWidth( 1 );
    pen.setColor( current_y );
    p->setPen( pen );
    p->drawLine( tail.x(), tail.y(), tail.x(), axisy_length );

    for( int i = 0; i < discY + 1; ++i )
    {
        double value = i*stepY;
        double dez = 0;

        if( value > 0 )
            dez = log10( value );
        p->drawLine( tail.x() - 3 , value, tail.x() + 3 , value );


        // the value to be written is a proportion of the real height of the axes with the discretization.
        // note that the axisy_length is scaled with the vertical exaggeration
        QString value1_ = QString::number( i*( height/discY), 'f', 1);
        p->rotate( -scale*90 );
        p->scale( 1, scale );
        p->drawText( QPointF( -scale*value - dez*0.1*stepY, tail.x() - 10 ), value1_ );
        p->scale( 1, scale );
        p->rotate( scale*90 );
    }

}

void CoordinateAxes2d::updateVerticalExaggeration( double scale_, double height_ )
{

    height = height_;
    axisy_length = scale_*height_;
    scale_on = true;
    update();
}

void CoordinateAxes2d::stopVerticalExaggeration(  double height_ )
{
    axisy_length = height_;
    scale_on = false;
    update();
}

void CoordinateAxes2d::resetVerticalExaggeration()
{
    axisy_length = height;
    scale_on = false;
    update();
}
