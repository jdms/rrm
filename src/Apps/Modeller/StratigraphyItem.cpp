#include "StratigraphyItem.hpp"

StratigraphyItem::StratigraphyItem ( QColor color ) : QGraphicsPathItem ( )
{
	this->prepareGeometryChange ( );

	curve = QPainterPath ( );

	this->color_ = color;

	setFlag ( QGraphicsItem::ItemIsSelectable );
	//setFlag ( QGraphicsItem::ItemIsMovable );

	setAcceptTouchEvents ( true );

}

StratigraphyItem::~StratigraphyItem ( )
{

}

void StratigraphyItem::paint ( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{
	painter->setRenderHint ( QPainter::Antialiasing );

	pen_color.setCapStyle(Qt::RoundCap);
	pen_color.setJoinStyle(Qt::RoundJoin);

	if ( isSelected ( ) )
	{
		pen_color.setWidth ( 3 );
		pen_color.setColor( Qt::green );
	}
	else
	{
		pen_color.setWidth ( 4 );
		pen_color.setColor ( this->color_ );
	}

	painter->setPen ( pen_color );

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


	curve.setFillRule(Qt::OddEvenFill);
}

/// Changed from original code
void StratigraphyItem::setSketch ( const QPolygonF & _path )
{
	this->prepareGeometryChange ( );

	curve = QPainterPath();

	curve.addPolygon(_path);

	curve.setFillRule(Qt::OddEvenFill);
}

QPainterPath StratigraphyItem::shape ( ) const
{
	// Right shape for picking
	QPainterPathStroker stroker;
	stroker.setWidth(20);
	return stroker.createStroke(curve);
}

void StratigraphyItem::setPen ( const QPen& pen )
{
	pen_color = pen;
}
