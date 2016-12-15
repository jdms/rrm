/*
 * RegionItem.cpp
 *
 *  Created on: Dec 13, 2016
 *      Author: felipe
 */

#include "RegionItem.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>

namespace RRM
{

	RegionItem::RegionItem() : QGraphicsItem()
	{
		setFlag(ItemIsMovable);
		setFlag(ItemSendsGeometryChanges);
		setCacheMode(DeviceCoordinateCache);
		setZValue(1);
	}

	QRectF RegionItem::boundingRect() const
	{
		qreal adjust = 5;

		QPolygon poly;
		poly << QPoint(0, -20);
		poly << QPoint(25, 0);
		poly << QPoint(0, 20);
		poly << QPoint(-25, 0); 

		return poly.boundingRect();

		//return QRectF(-10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
	}

	QPainterPath RegionItem::shape() const
	{
		QPainterPath path;
		path.addEllipse(-10, -10, 20, 20);
		return path;
	}

	void RegionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
	{
		painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
		painter->setRenderHints(QPainter::TextAntialiasing, true);
		// Revert QGraphicsView Scale to the default coordinate System.
		painter->scale(1, -1);
		// Shadow
		//painter->setPen(Qt::NoPen);
		//painter->setBrush(Qt::darkGray);
		//painter->drawEllipse(-7, -7, 20, 20);

		QRadialGradient gradient(-3, -3, 10);
		if (option->state & QStyle::State_Sunken) {
			gradient.setCenter(3, 3);
			gradient.setFocalPoint(3, 3);
			gradient.setColorAt(1, QColor(Qt::white).light(80));
			gradient.setColorAt(0, QColor(Qt::white).light(120));
		}
		else {
			gradient.setColorAt(0, Qt::white);
			gradient.setColorAt(1, QColor(Qt::white).light(80));
		}

		painter->setBrush(gradient);
		painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

		QPolygon poly;
		poly << QPoint(0, -12);
		poly << QPoint(10, 0);
		poly << QPoint(0, 12);
		poly << QPoint(-10, 0);

		painter->drawPolygon(poly);

		//painter->drawEllipse(-0, -0, 20, 20);

		painter->setFont(QFont("Arial", 8, QFont::Bold));

		// I have used the .adjusted() to make the text centralized on the diamond.
		painter->drawText(poly.boundingRect().adjusted(poly.boundingRect().width()*(-0.05), poly.boundingRect().height()*(-0.1), 0, 0), Qt::AlignCenter, QObject::tr("9"));
		
	}

	QVariant RegionItem::itemChange(GraphicsItemChange change, const QVariant &value)
	{
		switch (change) {
		case ItemPositionHasChanged:
			break;
		default:
			break;
		};

		return QGraphicsItem::itemChange(change, value);
	}

	void RegionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
	{
		update();
		QGraphicsItem::mousePressEvent(event);
	}

	void RegionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
	{
		update();
		QGraphicsItem::mouseReleaseEvent(event);
	}
}