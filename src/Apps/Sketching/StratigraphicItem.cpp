#include "StratigraphicItem.hpp"

StratigraphicItem::StratigraphicItem()
{
	prepareGeometryChange();
}


void StratigraphicItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* w )
{
	painter->setRenderHint( QPainter::Antialiasing );
	
    QPen pen_curve;
	
	pen_curve.setWidth( 3 );
	pen_curve.setColor( current_color );
	
	painter->setPen( pen_curve );
	painter->setBrush( Qt::NoBrush );
    painter->drawPolyline( points );
		
}


QRectF StratigraphicItem::boundingRect() const
{
	return curve.boundingRect();
}


QPainterPath StratigraphicItem::shape() const
{
	QPainterPath p;
	p.addRect( curve.boundingRect() );
    return p;
}


void StratigraphicItem::create( const QPointF& p )
{
	prepareGeometryChange();
	clear();
	
	points.push_back( p );	
}


void StratigraphicItem::add( const QPointF& p )
{
	prepareGeometryChange();
	points.push_back( p );
}


void StratigraphicItem::addSegment( const InputSketch& segment )
{
	QPolygonF cpy_polygon_segment = segment.getSketch();
	
	if( points.size() < 10 )
	{
		copySegment( cpy_polygon_segment );
		return;
	}
	
    Curve2D over_sketch = PolyQtUtils::qPolyginFToCurve2D( cpy_polygon_segment );
    Curve2D connected_sketches = PolyQtUtils::qPolyginFToCurve2D( points );
    Curve2D final, rest;
	
    connected_sketches = over_sketch.overSketch( connected_sketches, rest, 1 , 16 );
    connected_sketches.douglasPeuckerSimplify( final, 1.0 );
	
    copySegment( PolyQtUtils::curve2DToQPolyginF( final ) );
}


void StratigraphicItem::clear()
{
	prepareGeometryChange();

	points.clear();
	
}


void StratigraphicItem::copySegment( const QPolygonF& s )
{
	clear();
	
	points = QPolygonF( s );

}


void StratigraphicItem::update( const Eigen::Affine3f& m, const float &d )
{

    Curve2D* c = strat->getCurve( d );
    points.clear();


    unsigned int number_of_points = c->size();

    for( int i = 0; i < number_of_points; ++i )
    {
        Point2D p = c->at( i );
        Eigen::Vector4f p4d( p.x(), p.y(), d, 1.0f );

        p4d = m*p4d;

        points.push_back( QPointF( p4d.x(), p4d.y() ) );

    }



}

