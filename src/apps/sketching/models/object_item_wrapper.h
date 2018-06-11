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



#ifndef OBJECTITEMWRAPPER_H
#define OBJECTITEMWRAPPER_H


#include <QGraphicsPathItem>
#include <QColor>
#include <QPainter>


#include "./core/models/object.h"


class ObjectItemWrapper: public QGraphicsPathItem
{

    public:

        ObjectItemWrapper() = default;
        ObjectItemWrapper( Object* const& obj_, double depth_ );


        void setRawObject( Object* const& obj_ );
        Object* getRawObject() const ;

        std::size_t getIndex() const;

        void updateState();
        void updateCurve();
        void updateObject();
        void updateDepth( double depth_ );


        void setActive( bool status_ ){}
        bool isActive() const{}


        bool isVisible() const;
        bool isEditable() const;
        bool isSelectable() const;


        QRectF boundingRect() const;


        void clear();
        void clearData();
        void clearCurve();


        static QPainterPath polycurveToQPainterPath( const PolyCurve& pol_ );
        static QPolygonF vectorToQPolygonF( const std::vector< double >& vertices_ );
        static QPainterPath vectorToPainterPath( const std::vector< double >& vertices_,
                                                 const std::vector< std::size_t >& edges_ );



    protected:


        void setupPen();
        void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w );



    private:

        Object* raw;
        QPainterPath curve;
        QPen current_pen;

        double current_csection;



};

#endif // OBJECTITEMWRAPPER_H
