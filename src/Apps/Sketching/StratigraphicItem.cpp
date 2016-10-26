#include "StratigraphicItem.hpp"

StratigraphicItem::StratigraphicItem()
{
	prepareGeometryChange();

    setFlag( QGraphicsItem::ItemIsSelectable, false );
    setAcceptTouchEvents ( true );

    initSetup();
}


void StratigraphicItem::initSetup()
{

    under_operation = false;
    is_allowed = false;
    is_unallowed = true;
    is_selected = false;


    pen_allowed.setColor( Qt::yellow );
    pen_allowed.setWidth( 5 );

    pen_unallowed.setColor( Qt::gray );
    pen_unallowed.setWidth( 3 );

    pen_selected.setColor( Qt::red );
    pen_selected.setWidth( 5 );

    teste.setCapStyle( Qt::RoundCap );
    teste.setCurveThreshold( 0.5 );
    teste.setDashPattern( Qt::SolidLine );
    teste.setJoinStyle( Qt::RoundJoin );


}


void StratigraphicItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w )
{
	painter->setRenderHint( QPainter::Antialiasing );
	

    QBrush brush;

    if( under_operation == false )
    {
        pen_normal.setColor( current_color );
        pen_normal.setWidth( 3 );
        painter->setPen( pen_normal );

        brush = Qt::NoBrush;

    }
    else
    {

        brush = QBrush( current_color );

        if ( is_selected )
        {
            painter->setPen( pen_selected );
        }
        else if( is_allowed == true )
            painter->setPen( pen_allowed );

        else if( is_unallowed == true )
            painter->setPen( pen_unallowed );

    }

    painter->setBrush( brush );
    QPainterPath outline_curve = teste.createStroke( curve );

    painter->drawPath( outline_curve );






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


void StratigraphicItem::update( const Eigen::Affine3f& m, const float &d )
{


    Curve2D* c = strat->getCurve( d );

    points.clear();


    size_t number_of_points = c->size();

    for( size_t i = 0; i < number_of_points; ++i )
    {
        Point2D p = c->at( i );

        Eigen::Vector4f p4d( p.x(), p.y(), d, 1.0f );
        p4d = m*p4d;

        points.push_back( QPointF( p4d.x(), p4d.y() ) );

    }

    curve = QPainterPath();


    std::vector< unsigned int >& edges = strat->getCurveEdges();

    unsigned int id0 =0;
    unsigned int id1 =0;
    unsigned int last_id = 10000;

    size_t nedges = edges.size()/2;

    for( size_t i = 0; i < nedges; ++i )
    {

        id0 = edges[ 2*i ];
        id1 = edges[ 2*i + 1 ];

        if( last_id != id0 )
            curve.moveTo( points[ id0 ] );
        else
            curve.lineTo( points[ id1 ] );

        last_id = id1;
    }

 }

