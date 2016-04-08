#include "StratigraphyItem.hpp"

StratigraphyItem::StratigraphyItem ( QColor color ) : QGraphicsPathItem ( )
{
	this->prepareGeometryChange ( );

	curve = QPainterPath ( );

	pen_color = color;

	setFlag ( QGraphicsItem::ItemIsSelectable );
	setFlag ( QGraphicsItem::ItemIsMovable );

	setAcceptTouchEvents ( true );

}

StratigraphyItem::~StratigraphyItem ( )
{

}

void StratigraphyItem::paint ( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{
	painter->setRenderHint ( QPainter::Antialiasing );

	pen_color.setWidth ( 2 );

	if ( isSelected ( ) )
	{
		painter->setPen ( Qt::green );
	}
	else
	{
		painter->setPen ( pen_color );
	}

	painter->setBrush ( Qt::NoBrush );
	painter->drawPath ( curve );

}

QRectF StratigraphyItem::boundingRect ( ) const
{

	return curve.boundingRect ( );

}

void  StratigraphyItem::clear ( )
{
	this->prepareGeometryChange ( );
	curve = QPainterPath ( );

}

QPainterPath StratigraphyItem::getSketch ( ) const
{
	return curve;
}

/// Changed from original code
void StratigraphyItem::setSketch ( const QPainterPath & _path )
{
	this->prepareGeometryChange ( );

	curve = QPainterPath(_path);
}

QPainterPath StratigraphyItem::shape ( ) const
{
	return curve;
}

void StratigraphyItem::setPen ( const QPen& pen )
{
	pen_color = pen;
}
