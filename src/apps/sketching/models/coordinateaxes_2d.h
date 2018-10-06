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



#ifndef COORDINATEAXES2D_H
#define COORDINATEAXES2D_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QColor>

class CoordinateAxes2d: public QGraphicsLineItem
{
    public:

        enum class Plane{ XY, XZ, YZ };


        CoordinateAxes2d( const Plane& pl = Plane::XY );

        void setPlane( const Plane& pl );
        void setAxisXLenght( double l );
        void setAxisYLenght( double l );

        QRectF boundingRect() const;

    protected:

         void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
         void drawAxisX( QPainter* p );
         void drawAxisY( QPainter* p );

    protected:

        double axisx_length;
        double axisy_length;

        QString text_axisx;
        QString text_axisy;

        const QColor width_color = QColor( 255, 0, 0 );
        const QColor height_color = QColor( 0, 255, 0 );
        const QColor depth_color = QColor( 0, 0, 255 );

        QColor current_y;
        Plane plane;
        int scale;

};

#endif // COORDINATEAXES2D_H
