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



#include <limits>
#include <iostream>

#include "object_item_wrapper.h"


ObjectItemWrapper::ObjectItemWrapper( Object* const& obj_, double depth_ )
{
    setRawObject( obj_ );
    updateDepth( depth_ );
}



void ObjectItemWrapper::setRawObject( Object* const& obj_ )
{
    raw = obj_;
    setupPen();
    updateObject();
}


Object* ObjectItemWrapper::getRawObject() const
{
    return raw;
}



std::size_t ObjectItemWrapper::getIndex() const
{
    return raw->getIndex();
}


void ObjectItemWrapper::updateObject()
{
    updateState();
    updateCurve();
}


void ObjectItemWrapper::updateState()
{
    bool selectable_ = raw->isSelectable();
    bool selected_ = raw->isSelected();
    bool editable_ = raw->isEditable();
    int width_ = 3;

    int r, g, b;
    raw->getColor( r, g, b );


    QColor color_ = QColor( r, g, b );
    Qt::PenStyle style_ = Qt::SolidLine;


    if( selectable_  == true )
    {
        selectable_ = true;
        color_ = Qt::yellow;
    }


    if( selected_ == true )
    {
        color_ = color_.lighter();
        style_ = Qt::DashLine;
        width_ = 5;
    }


    current_pen.setColor( color_ );
    current_pen.setStyle( style_ );
    current_pen.setWidth( width_ );

//    setFlag( QGraphicsItem::ItemIsSelectable, ( selectable_ || editable_ ) );

    update();
}


void ObjectItemWrapper::updateCurve()
{

    if( raw->isEmpty() == true || raw->isActive() == false ) return;

    prepareGeometryChange();

    clearCurve();
    curve = ObjectItemWrapper::polycurveToQPainterPath( raw->getCurve( current_csection ) );
    setPath( curve );
    update();
}


void ObjectItemWrapper::updateDepth( double depth_ )
{
    current_csection = depth_;
    updateCurve();
}



bool ObjectItemWrapper::isEditable() const
{
    return raw->isEditable();
}


bool ObjectItemWrapper::isSelectable() const
{
    return raw->isSelectable();
}

bool ObjectItemWrapper::isVisible() const
{
    if( raw == nullptr ) return false;
    if( raw->isActive() == false ) return false;
    return raw->isVisible();
}




void ObjectItemWrapper::clear()
{
    clearCurve();
    clearData();
}


void ObjectItemWrapper::clearCurve()
{
    prepareGeometryChange();
    curve = QPainterPath();
}


void ObjectItemWrapper::clearData()
{
    raw = nullptr;
    current_csection = 0;
}




void ObjectItemWrapper::setupPen()
{
    current_pen.setStyle( Qt::SolidLine );
    current_pen.setCapStyle( Qt::RoundCap );
    current_pen.setJoinStyle( Qt::RoundJoin );
//    current_pen.setWidth( 3 );

}


QRectF ObjectItemWrapper::boundingRect() const
{
    return curve.boundingRect();
}


void ObjectItemWrapper::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
    if( isVisible() == false  ) return;
    if( isSelected() == true )
    {
        current_pen.setColor( Qt::blue );
    }
    if( raw->isDone() == false )
    {
        current_pen.setStyle( Qt::DotLine );
    }

    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( current_pen );
    painter->drawPath( curve );
}





QPainterPath ObjectItemWrapper::polycurveToQPainterPath( const PolyCurve& pol_ )
{

    QPainterPath path_;

//    std::vector< double > vertices_ = pol_.getVertices();
//    std::vector< std::size_t > edges_ = pol_.getEdges();

//    if( edges_.empty() == true )
//        path_.addPolygon( ObjectItemWrapper::vectorToQPolygonF( vertices_ ) );

//    else
//    {
//        path_ = ObjectItemWrapper::vectorToPainterPath( vertices_, edges_ );
//    }


    return path_;
}



QPolygonF ObjectItemWrapper::vectorToQPolygonF( const std::vector< double >& vertices_ )
{

    std::size_t size_ = vertices_.size()/2;

    QPolygonF pol_ ;
    for( std::size_t i = 0 ; i < size_ ; ++i )
        pol_ << QPointF( vertices_[ 2*i ], vertices_[ 2*i + 1 ] );

    return pol_;

}



QPainterPath ObjectItemWrapper::vectorToPainterPath( const std::vector< double >& vertices_,
                                                const std::vector< std::size_t >& edges_ )
{



    std::size_t number_of_edges_ = edges_.size()/2;
    std::size_t last_id_ = UINT_MAX;

//    for( auto id: edges_ )
//        std::cout << id << " " << std::flush;

//    std::cout << std::flush << std::endl;


    QPainterPath path_;
    for( std::size_t i = 0; i < number_of_edges_; ++i )
    {

        std::size_t id0_ = edges_[ 2*i ];
        std::size_t id1_ = edges_[ 2*i + 1 ];


        double x0 = vertices_[ 2*id0_ ];
        double y0 = vertices_[ 2*id0_ + 1 ];
        double x1 = vertices_[ 2*id1_ ];
        double y1 = vertices_[ 2*id1_ + 1 ];



//        std::cout << "Edge: "<< id0_ << " to " << id1_ << std::flush;
//        std::cout << "... p0: ( " << x0 << ", " << y0 <<
//                     "), p1: ( " << x1 << ", " << y1 << " )" << std::flush << std::endl;


        if( last_id_ != id0_ )
        {
//            std::cout << "movi para  " << id0_ << std::flush << std::endl;
            path_.moveTo( QPointF( x0, y0 ) );
        }
        else
        {
//            std::cout << "linha para " << id0_ << std::flush << std::endl;
            path_.lineTo( QPointF( x0, y0 ) );
        }

//        std::cout << "linha para id1 " << id1_ << std::flush << std::endl;
        path_.lineTo( QPointF( x1, y1 ) );

        last_id_ = id1_;

    }

    return path_;

}

