#include "Apps/Modeller/InputSketch.h"

InputSketch::InputSketch( QColor color ):QGraphicsPathItem()
{
    done = false;
    curve = QPainterPath();

    is_visible = false;
    is_inside = false;

    pen_color = color;

    setAcceptTouchEvents(true);


}

void InputSketch::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
    painter->setRenderHint( QPainter::Antialiasing );

//    pen_color.setWidth( 5 );
//
//
//    if( done == true ){
//        pen_color.setColor( QColor( 255, 0, 0 ) );
//        pen_color.setWidth( 5 );
//    }

    painter->setPen( pen_color );
    painter->setBrush( Qt::NoBrush );
    painter->drawPath( curve );

}

QRectF InputSketch::boundingRect() const
{

    return curve.boundingRect();

}

void InputSketch::create( const QPointF &p )
{
    this->prepareGeometryChange();
    curve.moveTo( p );
}

void InputSketch::add( const QPointF &p )
{
    this->prepareGeometryChange();
    curve.lineTo( p );
}

QRectF InputSketch::clear()
{
    this->prepareGeometryChange();
    QRectF rect = QRectF( -curve.boundingRect().width()/ 2, -curve.boundingRect().height()/ 2, curve.boundingRect().width(), curve.boundingRect().height());
    curve = QPainterPath();
    return rect;

}

void InputSketch::setDone( bool option )
{
    done = option;
}

bool InputSketch::isVisible() const
{
    return is_visible;
}


bool InputSketch::isInside() const
{
    return is_inside;
}

void InputSketch::isVisible( bool option )
{
    is_visible = option;
}

void InputSketch::isInside( bool option )
{
    is_inside = option;
}

QPainterPath InputSketch::getSketch()
{
    return curve;
}

/// Felipe
void  InputSketch::setSketch( const QPainterPath& p)
{
    this->prepareGeometryChange();
    curve = p;
}

QPainterPath InputSketch::shape() const
{

    return curve;
}

void InputSketch::setPen( const QPen& pen )
{
	pen_color = pen;
}
