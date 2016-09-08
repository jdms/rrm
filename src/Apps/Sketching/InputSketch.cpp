#include "InputSketch.h"

InputSketch::InputSketch ( QColor color ) :QGraphicsPathItem ( )
{
	this->prepareGeometryChange ( );

//	done = false;
//	curve = QPainterPath ( );

	is_visible = false;
	is_inside = false;

	pen_color = color;

	setFlag ( QGraphicsItem::ItemIsSelectable );
	setAcceptTouchEvents ( true );


}


void InputSketch::paint ( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{
	painter->setRenderHint ( QPainter::Antialiasing );


	pen_color.setWidth ( 3 );

	painter->setPen ( pen_color );

	painter->setBrush ( Qt::NoBrush );
    painter->drawPolyline( points_list );
//	painter->drawPath ( curve );

}


QRectF InputSketch::boundingRect ( ) const
{

    return points_list.boundingRect();//curve.boundingRect ( );

}


void InputSketch::create ( const QPointF &p )
{
	this->prepareGeometryChange ( );
	this->clear();
//	curve.moveTo ( p );
	points_list.push_back ( p );
}


void InputSketch::add ( const QPointF &p )
{
	this->prepareGeometryChange ( );
//	curve.lineTo ( p );
	points_list.push_back ( p );
}


//void InputSketch::addSegment( const InputSketch& segment )
//{


//    QPolygonF cpy_polygon_segment = segment.getSketch();

//    if( this->points_list.size() < 10 )
//    {

//        this->setSketch( cpy_polygon_segment );
//        return;
//    }


//    Curve2D over_sketch = PolyQtUtils::qPolyginFToCurve2D( cpy_polygon_segment );
//    Curve2D connected_sketches = PolyQtUtils::qPolyginFToCurve2D( this->points_list );
//    Curve2D final, rest;

//    connected_sketches = over_sketch.overSketch( connected_sketches, rest, 1, 16 );
//    connected_sketches.douglasPeuckerSimplify( final, 1.0 );

//    this->setSketch( PolyQtUtils::curve2DToQPolyginF( final ) );


//}


void InputSketch::clear ( )
{
	this->prepareGeometryChange ( );
//	curve = QPainterPath ( );
	points_list.clear ( );
}


void InputSketch::setDone ( bool option )
{
	done = option;
}


bool InputSketch::isVisible ( ) const
{
	return is_visible;
}


bool InputSketch::isInside ( ) const
{
	return is_inside;
}

void InputSketch::isVisible ( bool option )
{
	is_visible = option;
}


void InputSketch::isInside ( bool option )
{
	is_inside = option;
}


/// Changed from original code
void InputSketch::setSketch ( const QVector<QPointF> & _path )
{
    this->prepareGeometryChange ( );

    this->points_list.clear();
    this->points_list = QPolygonF(_path);

//    this->curve = QPainterPath();

//    this->curve.addPolygon(points_list);
}


/// Changed from original code
void InputSketch::setSketch ( const QPolygonF & _path )
{
    prepareGeometryChange ( );

    points_list.clear();
    points_list = QPolygonF(_path);

//    curve = QPainterPath();
//    curve.addPolygon( points_list );
}


/// Changed from original code
QPolygonF InputSketch::getSketch ( ) const
{
    return points_list;
}


///// Changed from original code
//QPainterPath InputSketch::getCurve ( ) const
//{
//	return curve;
//}


QPainterPath InputSketch::shape ( ) const
{

	QPainterPath path;

    path.addRect ( /*curve*/points_list.boundingRect ( ) );

	return path;
}


void InputSketch::setPen ( const QPen& pen )
{
	pen_color = pen;
}


void InputSketch::setColor(const QColor& _color)
{
	pen_color.setColor(_color);
}


QColor InputSketch::getColor() const
{
	return pen_color.color();
}

