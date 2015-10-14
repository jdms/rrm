#include "InputSketch.h"

InputSketch::InputSketch():QGraphicsPathItem()
{
    done = false;
    curve = QPainterPath();

    is_visible = false;
    is_inside = false;

    setAcceptTouchEvents(true);


}

void InputSketch::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
    painter->setRenderHint( QPainter::Antialiasing );

    QPen pen_color( QColor( 0, 0, 0 ) );
    pen_color.setWidth( 1 );


    if( done == true ){
        pen_color.setColor( QColor( 255, 0, 0 ) );
        pen_color.setWidth( 2 );
    }

    painter->setPen( pen_color );
    painter->setBrush( Qt::NoBrush );
    painter->drawPath( curve );

}

QRectF InputSketch::boundingRect() const
{

    return QRectF( -curve.boundingRect().width()/ 2, -curve.boundingRect().height()/ 2, curve.boundingRect().width(), curve.boundingRect().height());

}

void InputSketch::create( const QPointF &p )
{
    curve.moveTo( p );
}

void InputSketch::add( const QPointF &p )
{
    curve.lineTo( p );
}

QRectF InputSketch::clear()
{
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
