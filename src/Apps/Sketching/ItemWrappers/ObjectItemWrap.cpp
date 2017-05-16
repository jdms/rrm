#include "ObjectItemWrap.h"

ObjectItemWrap::ObjectItemWrap()
{
    object = nullptr;
    setFlag( QGraphicsItem::ItemIsSelectable, false );
}


void ObjectItemWrap::setupPens()
{

    visible_subpaths_pen.setStyle( Qt::SolidLine );
    visible_subpaths_pen.setCapStyle( Qt::RoundCap );
    visible_subpaths_pen.setJoinStyle( Qt::RoundJoin );
    visible_subpaths_pen.setWidth( 3 );


    hidden_subpaths_pen.setStyle( Qt::DotLine );
    hidden_subpaths_pen.setCapStyle( Qt::RoundCap );
    hidden_subpaths_pen.setJoinStyle( Qt::RoundJoin );
    hidden_subpaths_pen.setWidth( 3 );
}


void ObjectItemWrap::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w )
{

    if( object == nullptr ) return;
    if( object->getVisibility() == false ) return;

    visible_subpaths_pen.setColor( temp_color );
    hidden_subpaths_pen.setColor( temp_color );

    painter->setRenderHint( QPainter::Antialiasing );
    painter->setPen( visible_subpaths_pen );
    painter->drawPath( curve );
}

QRectF ObjectItemWrap::boundingRect() const
{
    return curve.boundingRect();
}

void ObjectItemWrap::clear()
{
    prepareGeometryChange();
    curve = QPainterPath();

    if( object != nullptr )
        object->clear();
    object = nullptr;

    intersection_points.clear();
    hidden_subpaths.clear();

}


void ObjectItemWrap::clearCurve()
{
    prepareGeometryChange();
    curve = QPainterPath();

    intersection_points.clear();
    hidden_subpaths.clear();

}

void ObjectItemWrap::setObjectRaw( Object* obj_, const double depth_  )
{
    object = obj_;

    int red_ = 0, green_ = 0, blue_ = 0;
    object->getColor( red_, green_, blue_ );
    current_color.setRgb( red_, green_, blue_ );

    updateCrossSection( depth_ );
}

void ObjectItemWrap::updateCrossSection( double depth_ )
{

    clearCurve();

    Curve2D c_ = object->getInputCurveofCrossSection( depth_ );
    std::vector< std::size_t > edges_ = object->getInputEdgesofCrossSection( depth_ );

    int r_, g_, b_;
    object->getColor( r_, g_, b_ );
    setColor( QColor( r_, g_, b_ ) );

    QPolygonF points_ = PolyQtUtils::curve2DToQPolyginF( c_ );

    if( edges_.empty() == true )
    {
        curve.addPolygon( points_ );
        setPath( curve );
        return;
    }

    std::size_t nedges_ = edges_.size()/2;
    std::size_t last_id = 10000;

    for( size_t i = 0; i < nedges_; ++i )
    {

        std::size_t id0 = edges_[ 2*i ];
        std::size_t id1 = edges_[ 2*i + 1 ];

        if( last_id != id0 )
            curve.moveTo( QPointF( points_[ id0 ].x(),
                                   points_[ id0 ].y() ) );
        else
            curve.lineTo( QPointF( points_[ id1 ].x(),
                                   points_[ id1 ].y() ) );


        last_id = id1;
    }

    setPath( curve );
    update();

}
