#include "ObjectItemWrap.h"

ObjectItemWrap::ObjectItemWrap()
{
    setupPens();
}


void ObjectItemWrap::setupPens()
{
    current_color = QColor( Qt::blue );

    visible_subpaths_pen.setColor( current_color );
    visible_subpaths_pen.setStyle( Qt::SolidLine );
    visible_subpaths_pen.setCapStyle( Qt::RoundCap );
    visible_subpaths_pen.setJoinStyle( Qt::RoundJoin );
    visible_subpaths_pen.setWidth( 3 );

    hidden_subpaths_pen.setColor( current_color );
    hidden_subpaths_pen.setStyle( Qt::DotLine );
    hidden_subpaths_pen.setCapStyle( Qt::RoundCap );
    hidden_subpaths_pen.setJoinStyle( Qt::RoundJoin );
    hidden_subpaths_pen.setWidth( 3 );
}


void ObjectItemWrap::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w )
{

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
}


void ObjectItemWrap::setObjectRaw( Object* obj_, const double depth_  )
{
    clear();

    Curve2D c_ = obj_->getInputCurveofCrossSection( depth_ );
    std::vector< std::size_t > edges_ = obj_->getInputEdgesofCrossSection( depth_ );

    QPolygonF points_ = PolyQtUtils::curve2DToQPolyginF( c_ );

    if( edges_.empty() == true )
    {
        curve.addPolygon( points_ );
        setPath( curve );
        return;
    }

    std::size_t nedges_ = edges_.size()/2;
    std::size_t last_id = 0;

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


}
