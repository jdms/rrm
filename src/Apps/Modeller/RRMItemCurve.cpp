/*
 * RRMItemCurve.cpp
 *
 *  Created on: Apr 6, 2016
 *      Author: felipe
 */

#include "RRMItemCurve.h"

RRMItemCurve::RRMItemCurve ( ) : QGraphicsPathItem()
{
	// TODO Auto-generated constructor stub

        setFlag(QGraphicsItem::ItemIsSelectable);
        setFlag(QGraphicsItem::ItemIsMovable);
        /// This flag is necessary for itemChange function work with ItemPositionChange  @see http://stackoverflow.com/a/8187925
        setFlag(QGraphicsItem::ItemSendsGeometryChanges);

}



RRMItemCurve::~RRMItemCurve ( )
{
	// TODO Auto-generated destructor stub
}

void RRMItemCurve::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
	if (isSelected())
	{
		 painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
	}else
	{
		painter->setPen(QPen(Qt::black, 1, Qt::SolidLine));
	}

	painter->drawPath(this->shape());
}


QRectF RRMItemCurve::boundingRect() const
{
	return this->shape().boundingRect();
}


QPainterPath RRMItemCurve::shape() const
{
	QPainterPath path;
	QPolygonF polygon;

	polygon << QPointF(0,0) << QPointF(0,10) << QPointF(0,20) << QPointF(20,20) << QPointF(20,40);

        path.addPolygon(polygon);

        return path;
}

