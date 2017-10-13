#include <limits>
#include <iostream>

#include "object_item_wrapper.h"


ObjectItemWrapper::ObjectItemWrapper( Object* const& obj_ )
{
    setRawObject( obj_ );
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




void ObjectItemWrapper::updateObject()
{
    updateState();
    updateCurve();
}


void ObjectItemWrapper::updateState()
{
    bool selectable_ = false;

    int r, g, b;
    raw->getColor( r, g, b );

    QColor color_ = QColor( r, g, b );
    Qt::PenStyle style_ = Qt::SolidLine;


    if( raw->isSelectable() == true )
    {
        selectable_ = true;
        color_ = Qt::yellow;
    }

    else if( raw->isSelected() == true )
    {
        color_ = color_.lighter();
        style_ = Qt::DotLine;
    }


    current_pen.setColor( color_ );
    current_pen.setStyle( style_ );

    setFlag( QGraphicsItem::ItemIsSelectable, selectable_ );
    update();
}


void ObjectItemWrapper::updateCurve()
{

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


void ObjectItemWrapper::setupPen()
{
    current_pen.setStyle( Qt::SolidLine );
    current_pen.setCapStyle( Qt::RoundCap );
    current_pen.setJoinStyle( Qt::RoundJoin );
    current_pen.setWidth( 3 );

}




bool ObjectItemWrapper::isVisible() const
{
    if( raw == nullptr ) return false;
    return raw->isVisible();

}




QRectF ObjectItemWrapper::boundingRect() const
{
    return curve.boundingRect();
}


void ObjectItemWrapper::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
    if( isVisible() == false  ) return;

    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( current_pen );
    painter->drawPath( curve );
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




QPainterPath ObjectItemWrapper::polycurveToQPainterPath( const PolyCurve& pol_ )
{

    QPainterPath path_;

    std::vector< double > vertices_;
    pol_.getVertices( vertices_ );

    std::vector< std::size_t > edges_;
    pol_.getEdges( edges_ );

    if( edges_.empty() == true )
        path_.addPolygon( ObjectItemWrapper::vectorToQPolygonF( vertices_ ) );

    else
        path_ = ObjectItemWrapper::vectorToPainterPath( vertices_, edges_ );


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

