#include "StratigraphicItem.hpp"

StratigraphicItem::StratigraphicItem()
{
	prepareGeometryChange();

    setFlag( QGraphicsItem::ItemIsSelectable, false );
    setAcceptTouchEvents ( true );
    setBoundingRegionGranularity( 0.9 );
    initSetup();
}


void StratigraphicItem::initSetup()
{

    under_operation = false;
    is_allowed = false;
    is_selected = false;


    pen_allowed.setColor( Qt::yellow );
    pen_allowed.setWidth( 5 );

    pen_selected.setColor( Qt::red );
    pen_selected.setWidth( 5 );

    setPath( curve );


}


void StratigraphicItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w )
{

	painter->setRenderHint( QPainter::Antialiasing );
	

    QBrush brush;
    QPen pen;
    pen.setColor( current_color );
    pen.setWidth( 3 );

    if( under_operation == true )
    {
        if( is_selected == true )
        {
            pen = pen_selected;
        }
        else if( is_allowed )
        {
            pen = pen_allowed;
        }

    }


    painter->setPen( pen );
    painter->setBrush( brush );
    painter->drawPath( curve );

}


QRectF StratigraphicItem::boundingRect() const
{
	return curve.boundingRect();
}


QPainterPath StratigraphicItem::shape() const
{
    return curve;
}


void StratigraphicItem::create( const QPointF& p )
{
	prepareGeometryChange();
	clear();
	
    points.push_back( p );
    curve.moveTo( p );
}


void StratigraphicItem::add( const QPointF& p )
{
	prepareGeometryChange();
	points.push_back( p );
    curve.lineTo( p );
}


QList< QPolygonF > StratigraphicItem::getSubCurves()
{
    return curve.toSubpathPolygons();
}


std::vector< Curve2D > StratigraphicItem::getSubCurves2D()
{
    QList< QPolygonF > subcurves = getSubCurves();

    size_t number_subcurves = subcurves.size();

    std::vector< Curve2D > subcurves2d;
    for( size_t i = 0; i  < number_subcurves; ++i )
    {
        subcurves2d.push_back( PolyQtUtils::qPolyginFToCurve2D( subcurves[ i ] ) );
    }

    return subcurves2d;

}


void StratigraphicItem::clear()
{
	prepareGeometryChange();

	points.clear();
    curve = QPainterPath();
	
}


void StratigraphicItem::copySegment( const QPolygonF& s )
{
    clear();
	
	points = QPolygonF( s );
    curve.addPolygon( points );

}


void StratigraphicItem::update( const QTransform& m, const float &d )
{


    Curve2D* c = strat->getCurve( d );

    points.clear();
    points = PolyQtUtils::curve2DToQPolyginF( *c );
    curve = QPainterPath();


    std::vector< size_t >& edges = strat->getCurveEdges();

    unsigned int id0 = 0;
    unsigned int id1 = 0;
    unsigned int last_id = 10000;

    size_t nedges = edges.size()/2;

    for( size_t i = 0; i < nedges; ++i )
    {

        id0 = edges[ 2*i ];
        id1 = edges[ 2*i + 1 ];

        if( last_id != id0 )
            curve.moveTo( QPointF( points[ id0 ].x()*m.m11() + m.dx(),
                                   points[ id0 ].y()*m.m22() + m.dy() ) );
        else
            curve.lineTo( QPointF( points[ id1 ].x()*m.m11() + m.dx(),
                                   points[ id1 ].y()*m.m22() + m.dy() ) );


        last_id = id1;
    }



    setPath( curve );
 }

