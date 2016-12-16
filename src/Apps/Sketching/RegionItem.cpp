/**
*  @file    RegionItem.hpp
*  @author  Felipe Moura de Carvalho (fmourade)
*  @date    13/12/2016
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
		this->setup();
	}

	RegionItem::RegionItem(int _id)
	{
		this->setup();
		this->id_ = _id;
	}

	void RegionItem::setup()
	{
		/// QGraphicsItem setup
		setFlag(ItemIsMovable);
		setFlag(ItemSendsGeometryChanges);
		setCacheMode(DeviceCoordinateCache);
		setZValue(1);
		/// Region setup
		this->id_ = -1;
	}

	QRectF RegionItem::boundingRect() const
	{
	
		QPolygon poly;
		poly << QPoint(0, -30);
		poly << QPoint(15, 0);
		poly << QPoint(0, 30);
		poly << QPoint(-15, 0); 

		return poly.boundingRect();
		
	}

	QPainterPath RegionItem::shape() const
	{
		QPainterPath path;

		QPolygon poly;
		poly << QPoint(0, -12);
		poly << QPoint(10, 0);
		poly << QPoint(0, 12);
		poly << QPoint(-10, 0);

		path.addPolygon(poly);
		return path;
	}

	void RegionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget * widget)
	{
		painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing, true);
		painter->setRenderHints(QPainter::TextAntialiasing, true);
		//@XXX Revert QGraphicsView Scale to the default coordinate System.
		painter->scale(1, -1);

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

		painter->drawEllipse(QPoint(0, -18), 3, 3);

		painter->setFont(QFont("Arial", 8, QFont::Bold));
		// I have used the .adjusted() to make the text centralized on the diamond.
		painter->drawText(poly.boundingRect().adjusted(poly.boundingRect().width()*(-0.05), poly.boundingRect().height()*(-0.1), 0, 0), Qt::AlignCenter,QString::number(this->id_));
		
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

	// Region setup
	void RegionItem::setID(const int _id)
	{
		this->id_ = _id;
	}
	int RegionItem::getID() const
	{
		return this->id_;
	}
}