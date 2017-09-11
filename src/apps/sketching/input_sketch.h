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



#ifndef INPUTSKETCH_H
#define INPUTSKETCH_H

#include <QtCore/QVector>
#include <QPainter>
#include <QGraphicsPathItem>
#include <iostream>


#include "sketching/polyqt_utils.hpp"

#include "./libs/SketchLibrary/SketchLib.hpp"


using namespace std;


/**!
 * @brief The line defined by the user during the sketch processing.
 */

class InputSketch : public QGraphicsPathItem
{


    public:


        InputSketch( QColor color );

        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option = 0, QWidget *w = 0 );
        QRectF boundingRect() const;

        void create( const QPointF& p );
        void add( const QPointF& p );
        void process( const QPointF& p );
        void clear();

        void setSketch( const QVector<QPointF> & _path );
        void setSketch( const QPolygonF & _path );
        QPolygonF getSketch() const;

        QPolygonF getSketchFunctionGraph() const;
        QPolygonF getSketchFunctionGraphTopView() const;

        static void clip( QPolygonF& pol_, int xmin_, int xmax_, int ymin_, int ymax_ );


        void setDone();

        bool isVisible() const;
        bool isInside() const;
        void isVisible( bool option );
        void isInside( bool option );

        QPainterPath shape() const;

        void setPen( const QPen& pen );
        void setColor( const QColor& _color);
        QColor getColor() const;

        inline bool isEmpty()
        {
            if( curve.toSubpathPolygons().empty() == true )
                return true;

            QList< QPolygonF > subcurves = curve.toSubpathPolygons();
            if ( subcurves[ 0 ].isEmpty() == true ) return true;
            if ( subcurves[ 0 ].size() < 5 ) return true;

            return false;
        }



    protected:

        QPen pen_color;
        QPainterPathStroker custom_stroker;

        QPainterPath curve;

        bool created;
        bool is_visible;
        bool is_inside;

        RRM::SketchLib sketchlib;





};

#endif // INPUTSKETCH_H
