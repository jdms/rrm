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
 * @file anglepicture.cpp
 * @author Clarissa C. Marques
 * @brief File containing the class AnglePicture
 */


#include <QPainter>
#include <QLinearGradient>
#include <QPainterPath>

#include "anglepicture.h"

AnglePicture::AnglePicture( const QSize& size_, double angle_ )
{
    //    pix = QPixmap( size_ );
    angle = angle_;
    size = size_;
    //    setPixmap( pix );
}

void AnglePicture::paintEvent(QPaintEvent * e)
{

    QLabel::paintEvent(e);

    // each coordinate axes cross each other in the middle
    int value = size.width();
    qreal xAxis = value/2;
    qreal yAxis = value/2;

    QPixmap pix( size );
    pix.fill( Qt::transparent );

    QPainter painter( &pix );

    QPen axes_pen_;
    axes_pen_.setWidth( 2 );
    axes_pen_.setColor( Qt::blue );

    // drawing the X axes with blue color
    painter.setPen( axes_pen_ );
    painter.drawLine(0, xAxis, value,  xAxis);

    // drawing the Y axes with red color
    axes_pen_.setColor( Qt::red );
    painter.setPen( axes_pen_ );
    painter.drawLine(yAxis, 0, yAxis, value);


    QPointF center = QPointF(yAxis, xAxis);

    // tracing a line from the origin and with angle 'angle' relative to the x axes
    QLineF angleline;
    angleline.setP1( center);
    angleline.setAngle( angle );
    angleline.setLength(value);

    // tracing a line from the origin and with angle '180 - angle' relative to the x axes
    QLineF angleline1;
    angleline1.setP1( center);
    angleline1.setAngle( 180 - angle );
    angleline1.setLength(value);

    // tracing a line from the origin and with angle '180 + angle' relative to the x axes
    QLineF angleline2;
    angleline2.setP1( center);
    angleline2.setAngle( 180 + angle );
    angleline2.setLength(value);

    // tracing a line from the origin and with angle '360 - angle' relative to the x axes
    QLineF angleline3;
    angleline3.setP1( center);
    angleline3.setAngle( 360 - angle );
    angleline3.setLength(value);


    // drawing the lines above
    QPen line_pen_;
    line_pen_.setColor( Qt::black );
    line_pen_.setWidth( 1 );
    painter.setPen( line_pen_ );
    painter.drawLine(angleline);
    painter.drawLine(angleline1);
    painter.drawLine(angleline2);
    painter.drawLine(angleline3);

    QPainterPath myPath, myPath1, myPath2, myPath3;
    myPath.moveTo(center);
    myPath1.moveTo(center);
    myPath2.moveTo(center);
    myPath3.moveTo(center);

    double x_ = center.x() - value/6;
    double y_ = center.y() - value/6;
    double w_ = 2*value/6;
    double h_ = 2*value/6;


    // drawing arcs to represent the angles
    myPath.arcTo( x_, y_, w_, h_, 0, angle );
    myPath1.arcTo( x_, y_, w_, h_, 180, -angle );
    myPath2.arcTo( x_, y_, w_, h_, 180, angle );
    myPath3.arcTo( x_, y_, w_, h_, 0, -angle );
//    myPath.arcTo( pix.rect() , 0, angle );
    painter.setPen( Qt::black );
    painter.setBrush( QBrush( QColor( 255, 255, 0, 125 ) ) );
    painter.drawPath(myPath);
    painter.drawPath(myPath1);
    painter.drawPath(myPath2);
    painter.drawPath(myPath3);

    painter.setRenderHint(QPainter::Antialiasing, true );
    setPixmap( pix );
}

void AnglePicture::updateAngle( double angle_ )
{
    angle = angle_;
    repaint();
}
