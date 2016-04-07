/*
 * StatigraphyController.hpp
 *
 *  Created on: Apr 6, 2016
 *      Author: felipe
 */

#ifndef _VIEW_STATIGRAPHYCONTROLLER_HPP_
#define _VIEW_STATIGRAPHYCONTROLLER_HPP_

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

		QPainterPath getSketch ( );
		void setSketch ( const QPainterPath& p );

		QPainterPath shape ( ) const;

		void setPen ( const QPen& pen );

	        void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	protected:

		QPen pen_color;

		QPainterPath curve;



};

#endif /* _VIEW_STATIGRAPHYCONTROLLER_HPP_ */
