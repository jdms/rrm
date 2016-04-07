/*
 * StratigraphyController.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: felipe
 */

#include "StratigraphyItem.hpp"

StratigraphyItem::StratigraphyItem ( QColor color ) :QGraphicsPathItem ( )
{
	curve = QPainterPath ( );

	pen_color = color;

	setFlag ( QGraphicsItem::ItemIsSelectable,true );

	setAcceptTouchEvents ( true );

}

StratigraphyItem::~StratigraphyItem ( )
{

}

QPainterPath StratigraphyItem::shape ( ) const
{
	QPainterPath p;
	p.addRect(curve.boundingRect());
	return p;
}

QRectF StratigraphyItem::boundingRect ( ) const
{

	return this->shape().boundingRect();

}

void StratigraphyItem::paint ( QPainter *painter , const QStyleOptionGraphicsItem *option , QWidget *w )
{
	painter->setRenderHint ( QPainter::Antialiasing );

	pen_color.setWidth ( 3 );

	if ( this->isSelected ( ) )
	{
		painter->setPen ( Qt::black );
	}
	else
	{
		painter->setPen ( pen_color );
	}

	painter->setBrush ( Qt::NoBrush );

	painter->drawRect(curve.boundingRect ( ));
}

QPainterPath StratigraphyItem::getSketch ( )
{
	return curve;
}

/// Changed from original code
void StratigraphyItem::setSketch ( const QPainterPath& p )
{
	this->prepareGeometryChange ( );
	curve = p;
}

void StratigraphyItem::setPen ( const QPen& pen )
{
	pen_color = pen;
}

void StratigraphyItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}
