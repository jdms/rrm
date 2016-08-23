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


void InputSketch::addSegment( const InputSketch& segment )
{


    QPolygonF cpy_polygon_segment = segment.getSketch();

    if( this->input_line_.size() < 10 )
    {

        this->setSketch( cpy_polygon_segment );
        return;
    }


    Curve2D over_sketch = convert( cpy_polygon_segment );
    Curve2D connected_sketches = convert( this->input_line_ );
    Curve2D final, rest;

    connected_sketches = over_sketch.overSketch( connected_sketches, rest, 1, 16 );
    connected_sketches.douglasPeuckerSimplify( final, 1.0 );

    this->setSketch( convert( final ) );


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

    this->input_line_.clear();
    this->input_line_ = QPolygonF(_path);

    this->curve = QPainterPath();

    this->curve.addPolygon(input_line_);
}


/// Changed from original code
void InputSketch::setSketch ( const QPolygonF & _path )
{
    prepareGeometryChange ( );

    input_line_.clear();
    input_line_ = QPolygonF(_path);

    curve = QPainterPath();
    curve.addPolygon( input_line_ );
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


InputSketch::Curve2D InputSketch::convert(QPolygonF _curve)
{
    Curve2D rrm_curve;

    for (auto p : _curve)
    {
        rrm_curve.push_back(Point2D(p.x(), p.y()));
    }

    return rrm_curve;
}


/// Model Related Function
QPolygonF InputSketch::convert(Curve2D _curve)
{
    QPolygonF qt_curve;

    for (std::size_t p_it = 0; p_it < _curve.size(); p_it++)
    {
        qt_curve.push_back(QPointF(_curve[p_it].x(), _curve[p_it].y()));
    }

    return qt_curve;
}
