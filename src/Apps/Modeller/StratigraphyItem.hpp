/*
 * StatigraphyController.hpp
 *
 *  Created on: Apr 6, 2016
 *      Author: felipe
 */

#ifndef _VIEW_STATIGRAPHYItem_HPP_
#define _VIEW_STATIGRAPHYItem_HPP_

#include <QPainter>
#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QStyle>
#include <QStyleOptionGraphicsItem>
#include <iostream>


class StratigraphyItem : public QGraphicsPathItem
{

	public:

		StratigraphyItem( QColor color );

		virtual ~StratigraphyItem ( );

		void paint ( QPainter *painter , const QStyleOptionGraphicsItem *option = 0 , QWidget *w = 0 );
		QRectF boundingRect ( ) const;

		QPainterPath getSketch ( ) const;
		void setSketch ( const QPainterPath& _path );
		void setSketch ( const QPolygonF&    _path );

		void clear();

		QPainterPath shape ( ) const;

		void setPen ( const QPen& pen );

	protected:

		QPen pen_color;

		QPainterPath curve;
};

#endif /* _VIEW_STATIGRAPHYItem_HPP_ */

