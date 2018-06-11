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



#include "trajectoryitemwrapper.h"



TrajectoryItemWrapper::TrajectoryItemWrapper( Object* const& obj_ )
{
    setRawObject( obj_ );
}

void TrajectoryItemWrapper::setRawObject( Object* const& obj_ )
{
    raw = obj_;
    setupPen();
    updateTrajectory();
}


Object* TrajectoryItemWrapper::getRawObject() const
{
    return raw;
}



std::size_t TrajectoryItemWrapper::getIndex() const
{
    return raw->getIndex();
}


void TrajectoryItemWrapper::updateTrajectory()
{
    updateState();
    updateCurve();
}


void TrajectoryItemWrapper::updateState()
{
    bool selectable_ = raw->isSelectable();
    bool selected_ = raw->isSelected();
    bool editable_ = raw->isEditable();

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
        style_ = Qt::DotLine;
    }



    current_pen.setColor( color_ );
    current_pen.setStyle( style_ );

    setFlag( QGraphicsItem::ItemIsSelectable, ( selectable_ || editable_ ) );

    update();
}


void TrajectoryItemWrapper::updateCurve()
{

    if( raw->isEmpty() == true || raw->isActive() == false ) return;

    prepareGeometryChange();

    clearCurve();
    curve = TrajectoryItemWrapper::polycurveToQPainterPath( raw->getTrajectory() );
    setPath( curve );
    update();
}


void TrajectoryItemWrapper::updateDepth( double depth_ )
{
    current_csection = depth_;
    updateCurve();
}



bool TrajectoryItemWrapper::isEditable() const
{
    return raw->isEditable();
}


bool TrajectoryItemWrapper::isVisible() const
{
    if( raw == nullptr ) return false;
    if( raw->isActive() == false ) return false;
    return raw->isVisible();
}




void TrajectoryItemWrapper::clear()
{
    clearCurve();
    clearData();
}


void TrajectoryItemWrapper::clearCurve()
{
    prepareGeometryChange();
    curve = QPainterPath();
}


void TrajectoryItemWrapper::clearData()
{
    raw = nullptr;
    current_csection = 0;
}




void TrajectoryItemWrapper::setupPen()
{
    current_pen.setStyle( Qt::SolidLine );
    current_pen.setCapStyle( Qt::RoundCap );
    current_pen.setJoinStyle( Qt::RoundJoin );
    current_pen.setWidth( 3 );

}


QRectF TrajectoryItemWrapper::boundingRect() const
{
    return curve.boundingRect();
}


void TrajectoryItemWrapper::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
    if( isVisible() == false  ) return;
    if( isSelected() == true )
    {
        current_pen.setColor( Qt::blue );
    }

    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( current_pen );
    painter->drawPath( curve );
}





QPainterPath TrajectoryItemWrapper::polycurveToQPainterPath( const PolyCurve& pol_ )
{

    QPainterPath path_;

    std::vector< double > vertices_ = pol_.getVertices();
    std::vector< std::size_t > edges_ = pol_.getEdges();

    if( edges_.empty() == true )
        path_.addPolygon( TrajectoryItemWrapper::vectorToQPolygonF( vertices_ ) );

    else
        path_ = TrajectoryItemWrapper::vectorToPainterPath( vertices_, edges_ );


    return path_;
}



QPolygonF TrajectoryItemWrapper::vectorToQPolygonF( const std::vector< double >& vertices_ )
{

    std::size_t size_ = vertices_.size()/2;

    QPolygonF pol_ ;
    for( std::size_t i = 0 ; i < size_ ; ++i )
        pol_ << QPointF( vertices_[ 2*i ], vertices_[ 2*i + 1 ] );

    return pol_;

}



QPainterPath TrajectoryItemWrapper::vectorToPainterPath( const std::vector< double >& vertices_,
                                                const std::vector< std::size_t >& edges_ )
{

    std::size_t number_of_edges_ = edges_.size()/2;
    std::size_t last_id_ = UINT_MAX;

    QPainterPath path_;
    for( std::size_t i = 0; i < number_of_edges_; ++i )
    {

        std::size_t id0_ = edges_[ 2*i ];
        std::size_t id1_ = edges_[ 2*i + 1 ];

        if( last_id_ != id0_ )
            path_.moveTo( QPointF( vertices_[ 2*id0_ ],
                                   vertices_[ 2*id0_ + 1 ] ) );
        else
            path_.lineTo( QPointF( vertices_[ 2*id1_ ],
                                   vertices_[ 2*id1_ + 1 ] ) );

        last_id_ = id1_;

    }

    return path_;

}

