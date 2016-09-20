#include "InputSketch.h"

InputSketch::InputSketch ( QColor color ) :QGraphicsPathItem ( )
{
    prepareGeometryChange ( );


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

}


QRectF InputSketch::boundingRect ( ) const
{

    return points_list.boundingRect();

}


void InputSketch::create ( const QPointF &p )
{
    prepareGeometryChange ( );
    clear();
	points_list.push_back ( p );
}


void InputSketch::add ( const QPointF &p )
{
    prepareGeometryChange ( );
	points_list.push_back ( p );
}





void InputSketch::clear ( )
{
    prepareGeometryChange ( );
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
    prepareGeometryChange ( );

    points_list.clear();
    points_list = QPolygonF(_path);

}


/// Changed from original code
void InputSketch::setSketch ( const QPolygonF & _path )
{
    prepareGeometryChange ( );

    points_list.clear();
    points_list = QPolygonF(_path);

}


/// Changed from original code
QPolygonF InputSketch::getSketch ( ) const
{
    return points_list;
}


QPainterPath InputSketch::shape ( ) const
{

	QPainterPath path;

    path.addRect ( points_list.boundingRect ( ) );

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

