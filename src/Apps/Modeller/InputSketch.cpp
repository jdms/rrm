#include "InputSketch.h"

InputSketch::InputSketch ( QColor color ) :QGraphicsPathItem ( )
{
	this->prepareGeometryChange ( );

	done = false;
	curve = QPainterPath ( );

	is_visible = false;
	is_inside = false;

	pen_color = color;

	setFlag ( QGraphicsItem::ItemIsSelectable );

	setAcceptTouchEvents ( true );

}

void InputSketch::paint ( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{
	painter->setRenderHint ( QPainter::Antialiasing );

//    pen_color.setWidth( 5 );

	pen_color.setWidth ( 3 );

	painter->setPen ( pen_color );

	painter->setBrush ( Qt::NoBrush );
	painter->drawPath ( curve );

}

QRectF InputSketch::boundingRect ( ) const
{

	return curve.boundingRect ( );

}

void InputSketch::create ( const QPointF &p )
{
	this->prepareGeometryChange ( );
	this->clear();
	curve.moveTo ( p );
	input_line_.push_back ( p );
}

void InputSketch::add ( const QPointF &p )
{
	this->prepareGeometryChange ( );
	curve.lineTo ( p );
	input_line_.push_back ( p );
}

void InputSketch::clear ( )
{
	this->prepareGeometryChange ( );
	curve = QPainterPath ( );
	input_line_.clear ( );
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

	input_line_.clear();
	input_line_ = QPolygonF(_path);

	curve = QPainterPath();

	curve.addPolygon(input_line_);
}
/// Changed from original code
void InputSketch::setSketch ( const QPolygonF & _path )
{
	this->prepareGeometryChange ( );

	input_line_.clear();
	input_line_ = QPolygonF(_path);

	curve = QPainterPath();

	curve.addPolygon(input_line_);
}


/// Changed from original code
QPolygonF InputSketch::getSketch ( ) const
{
	return input_line_;
}
/// Changed from original code
QPainterPath InputSketch::getCurve ( ) const
{
	return curve;
}

QPainterPath InputSketch::shape ( ) const
{

	QPainterPath path;

	path.addRect ( curve.boundingRect ( ) );

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